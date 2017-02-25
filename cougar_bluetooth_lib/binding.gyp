{
  "targets": [
    {
      "target_name": "cougar_bluetooth_lib",
      "sources": [
        "cougar_bluetooth_lib.cc",
        
        "cougar_bluetooth_interface.h",
        "cougar_bluetooth_osx.h",
        
        
        
        "cougar_bluetooth.h",
        "cougar_bluetooth.mm",
        
        "./macOS/cougar_bluetooth_osx.h",
        "./macOS/cougar_bluetooth_osx.mm"
       ]
    }
  ],
  "include_dirs" : [
    "<!(node -e \"require('nan')\")",
  ],
  "conditions": [
      ['OS=="linux"',
        {
        }
      ],
      ['OS=="mac"',
        {
          'defines': [
          '__MACOSX_CORE__'
          ],
          'link_settings': {
              'libraries': [
              '-framework', 'CoreBluetooth',
              '-framework', 'CoreFoundation',
              ]
          },
          'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
              'HEADER_SEARCH_PATHS' : "<!(node -e \"require('nan')\")",
          },
          'include_dirs' : [
            "<!(node -e \"require('nan')\")",
          ],
        }
      ],
      ['OS=="win"',
        {
        }
      ]
    ],
    'xcode_settings': {
        'OTHER_CFLAGS': [
            '-ObjC',
            '-std=c++11'
        ],
    },
}
