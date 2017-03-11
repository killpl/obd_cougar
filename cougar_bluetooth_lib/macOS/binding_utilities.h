//
//  Header.h
//  binding
//
//  Created by Adam Kozłowski on 27/02/2017.
//
//

#pragma once

// Utility - easier to change if necessary

#define STLStringToNSString(str) [NSString stringWithUTF8String:str.c_str()]

#define NSStringToSTLString(str) [str UTF8String]

