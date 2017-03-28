{
  "targets": [
    {
      "target_name": "obd_cougar",
      
      "sources": [
        "obd_cougar_lib.cc",
        
        "./../cougar_lib_commons/Logger.h",
        "./../cougar_lib_commons/Listenable.h",
        
        "./serial/Interfaces.h",
        "./serial/Communication.h",
        "./serial/macOS/InterfacesOSX.h",
        "./serial/macOS/InterfacesOSX.cpp"
       ],

       "include_dirs" : [
          "<!(node -e \"require('nan')\")"
       ],

       "conditions": [
          
          ['OS=="mac"',
            {
              'defines': [
                '__MACOSX_CORE__'
              ],
              'link_settings': {
                  'libraries': [
                    '-framework IOKit',
                    '-framework CoreFoundation',
                  ]
              },
              'xcode_settings': {
                  'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                  'CLANG_CXX_LIBRARY': 'libc++',

                  'OTHER_CFLAGS': [
                    '-ObjC++',
                    '-std=c++11'
                  ],
              },
            }
          ],

          ['OS=="win"',
            {
            }
          ],

          ['OS=="linux"',
            {
            }
          ],
      ],

    }
  ],
}
