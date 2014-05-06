//
//  rle.cc
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/11/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

/*

 
 
 */


#include <rp/drivers/display/rpusbdisp/rle.h>
#include <rp/util/int_types.h>
#include <rp/util/buffer.h>
#include <rp/util/scopes.h>
#include <rp/deps/libusbx_wrap/libusbx_wrap.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace rp::deps::libusbx_wrap;
using namespace rp::util;

namespace rp { namespace drivers { namespace display {
    

    shared_ptr<Buffer> rleCompress(shared_ptr<Buffer> buffer) {
        if (buffer->size() & 0x1) {
            throw Exception(-1, "Rle should align in 2 bytes");
        }
        
        // estimate the buffer usage in the worst case
        // the worst case is 1 extra byte with each 128 pixels (since the 7bit size block can represent 128 uints)
        
        size_t estBufferSize = buffer->size() + (((buffer->size()>>1) + 0x7f)>>7);
        _u8 *  outputBuffer = new _u8[estBufferSize];
        
        BufferLockScope scope(buffer);
        
        _u16 * pixelData = (_u16 *)scope.getBuffer();
        
        
        //FIXME: assumes the pixel data is 16bit is not always true for the future rpusbdisp products
        size_t pixelPos = 0;
        _u16 lastPixelValue;
        _u8 * encodingPos    = outputBuffer;
        _u8 * sectionHeader; //section header is used to store the section length and section type
        const size_t totoalPixels = (buffer->size()>>1);
        
        
        // initial setup...
        sectionHeader = encodingPos++;
        *sectionHeader = 0; //set the initial section length to 1 (zero section length is not allowed, so 0 actually means 1)
        lastPixelValue = pixelData[pixelPos++];
        *((_u16 *)encodingPos) = lastPixelValue; //copy the first pixel into it
        encodingPos +=2;
    
        while (pixelPos < totoalPixels) {
        
            bool isCommonSection = *sectionHeader & 0x80;
            if (isCommonSection) {
                while (pixelPos < totoalPixels) {
                    _u16 currentPixel = pixelData[pixelPos++];
                    
                    if (currentPixel == lastPixelValue) {
                        if (*sectionHeader == (0x7f | 0x80)) {
                            // no room for the current section, create a new one
                            sectionHeader = encodingPos;
                            ++encodingPos;
                            *sectionHeader = 0x80;
                            *((_u16 *)encodingPos) = currentPixel;
                            encodingPos+=2;
                        } else {
                            ++(*sectionHeader);
                        }
                        
                    } else {
                        // create a new section
                        sectionHeader = encodingPos;
                        ++encodingPos;
                        *sectionHeader = 0;
                        *((_u16 *)encodingPos) = currentPixel;
                        encodingPos+=2;
                        
                        lastPixelValue = currentPixel;
                        break;
                    }
                }
                
                
            } else {
                while (pixelPos < totoalPixels) {
                    _u16 currentPixel = pixelData[pixelPos++];
                    
                    if (currentPixel == lastPixelValue) {
                        // find two common pixels
                        if (* sectionHeader == 0) {
                            // change the current section into a common section directly...
                            ++(*sectionHeader);
                            *sectionHeader |= 0x80;
                        } else {
                            // form a new section
                            
                            //remove the prevous pixel data from the current section
                            (*sectionHeader)--;
                            encodingPos-=2;
                            
                            
                            // init the new section
                            sectionHeader = encodingPos;
                            encodingPos ++;
            
                            *sectionHeader = 0x80 + 1;
                            *(_u16 *)encodingPos = currentPixel;
                            encodingPos+=2;
                            
                        }
                        break;
                    } else {
                        if (*sectionHeader == 0x7f) {
                            // no more space in current header, create a new one...
                            sectionHeader = encodingPos;
                            ++encodingPos;
                            *sectionHeader = 0;
                        } else {
                            ++(*sectionHeader);
                        }
                        
                        // push the current pixel data to the section
                        *(_u16 *)encodingPos = currentPixel;
                        encodingPos+=2;
                        lastPixelValue = currentPixel;
                    }
                    
                }
                
            }
            
            
        }
        
        size_t outputBufferSize = encodingPos - outputBuffer;
    
        
        shared_ptr<Buffer> outputTransferBuffer(new Buffer(outputBufferSize));
        
        BufferLockScope outputScope(outputTransferBuffer);
        memcpy(outputScope.getBuffer(), outputBuffer, outputBufferSize);
        delete [] outputBuffer;
        return outputTransferBuffer;
    }
    
}}}
