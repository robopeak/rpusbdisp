//
//  exception.h
//  rpusbdispsdk
//
//  Created by Tony Huang on 12/17/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

#include <rp/infra_config.h>
#include <rp/util/noncopyable.h>
#include <string>

namespace rp { namespace util {
    
    /**
     * \brief The Exception class
     *
     * The C++ interface of RoboPeak Mini USB Display use exceptions to pass errors (instead of the legacy, hard-to-use return code method). Exception class instance is the only exception type the SDK will throw.
     *
     * Example
     * \code{.cpp}
     * try {
     *     // some calls to the sdk
     * } catch (Exception& e) {
     *     // deal with the error
     * }
     * \endcode
     */
	class RP_INFRA_API Exception {
    public:
        /**
         * Construct an exception
         *
         * \param errorCode The error code
         * \param name The name of the error
         * \param description Detailed information of the error
         */
        Exception(int errorCode, const std::string& name = "", const std::string& description = "");
        
        /**
         * Construct an exception by cloning another one
         */
        Exception(const Exception& that);
        
        ~Exception();
        
        Exception& operator=(const Exception& that);

        /**
         * Get the error code
         */
        int errorCode() const;
        
        /**
         * Get the name of the error
         */
        const std::string& name() const;
        
        /**
         * Get detailed information of the error
         */
        const std::string& description() const;
        
        /**
         * Format to friendly string
         */
        std::string toString() const;
        
        /**
         * Print error to the console (std::cerr)
         */
        void printToConsole() const;
        
    private:
        int errorCode_;
        std::string name_;
        std::string description_;
    };
    
}}
