cmd_Release/cougar_bluetooth_lib.node := c++ -bundle -undefined dynamic_lookup -Wl,-no_pie -Wl,-search_paths_first -mmacosx-version-min=10.7 -arch x86_64 -L./Release -stdlib=libc++  -o Release/cougar_bluetooth_lib.node Release/obj.target/cougar_bluetooth_lib/cougar_bluetooth_lib.o Release/obj.target/cougar_bluetooth_lib/macOS/cougar_bluetooth.o Release/obj.target/cougar_bluetooth_lib/macOS/cougar_bluetooth_osx.o -framework CoreBluetooth -framework CoreFoundation