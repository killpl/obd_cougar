{
  "targets": [
    {
      "target_name": "cougar_bluetooth_lib",
      "sources": [ "cougar_bluetooth_lib.cc" ]
    }
  ],
  "include_dirs" : [
    "<!(node -e \"require('nan')\")"
  ],
  'conditions': [
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
              '-framework', 'IOBluetooth',
              '-framework', 'CoreFoundation',
              ]
          },
          'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }
      ],
      ['OS=="win"',
        {
        }
      ]
    ]
}
