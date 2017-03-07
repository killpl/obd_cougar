{
  "targets": [
    {
      "target_name": "cougar_bluetooth_lib",
      
      "sources": [
        "cougar_bluetooth_lib.cc",
        
        "cougar_bluetooth_interface.h",
        
        "./../cougar_lib_commons/Listenable.h",
        
        "./macOS/cougar_bluetooth.h",
        "./macOS/cougar_bluetooth.mm",
        
        "./macOS/cougar_bluetooth_osx.h",
        "./macOS/cougar_bluetooth_osx.mm"
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
                    'CoreBluetooth.framework',
                    'CoreFoundation.framework',
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
