var electron, path, json;

path = require('path');
json = require('../../package.json');

electron = require('electron');

bluetooth = require("./../../../../cougar_bluetooth_lib/build/Release/cougar_bluetooth_lib.node");


var _logFunction = function(str) {
  console.log(str);
};
console.log(bluetooth);
bluetooth.SetLogCallback(_logFunction);

b = new bluetooth.Cougar_Bluetooth();



electron.app.on('ready', function() {
  var window;

  window = new electron.BrowserWindow({
    title: json.name,
    width: json.settings.width,
    height: json.settings.height
  });

  window.loadURL('file://' + path.join(__dirname, '..', '..') + '/index.html');

  window.webContents.on('did-finish-load', function(){
    window.webContents.send('loaded', {
      appName: json.name,
      electronVersion: process.versions.electron,
      nodeVersion: process.versions.node,
      chromiumVersion: process.versions.chrome
    });

    console.log(b.GetStatus());
    b.StartScan("", _logFunction);
  });

  window.on('closed', function() {
    window = null;
  });

});
