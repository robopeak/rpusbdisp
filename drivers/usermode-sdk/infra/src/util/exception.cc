//
//  exception.cc
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/17/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <rp/infra_config.h>
#include <rp/util/exception.h>

using namespace std;

namespace rp { namespace util {
    
    Exception::Exception(int errorCode, const string& name, const string& description)
    : errorCode_(errorCode), name_(name), description_(description)
    {}
    
    Exception::Exception(const Exception& that) : errorCode_(that.errorCode_), name_(that.name_), description_(that.description_) {}
    
    Exception::~Exception() {}
    
    Exception& Exception::operator=(const rp::util::Exception &that) {
        errorCode_ = that.errorCode_;
        name_ = that.name_;
        description_ = that.description_;
        
        return *this;
    }
    
    int Exception::errorCode() const {
        return errorCode_;
    }
    
    const string& Exception::name() const {
        return name_;
    }
    
    const string& Exception::description() const {
        return description_;
    }
    
    string Exception::toString() const {
        string name = name_;
        if (name.empty()) {
            char errorMessage[64];
            snprintf(errorMessage, 64, "Error code: 0x%08x", errorCode_);
            name = errorMessage;
        }
        
        return description_.empty() ? name : (name + "\n" + description_);
    }
    
    void Exception::printToConsole() const {
        string errorMessage = toString();
        
        fprintf(stderr, "%s\n", errorMessage.c_str());
    }
    
}}

