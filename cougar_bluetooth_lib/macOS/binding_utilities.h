//
//  Header.h
//  binding
//
//  Created by Adam Kozłowski on 27/02/2017.
//
//

#pragma once

// Utility - easier to change if necessary

#define NSStringToSTLString(str) [NSString stringWithUTF8String:str.c_str()];

#define STLStringToNSString(str) [str UTF8String];

