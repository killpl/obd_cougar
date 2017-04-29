
// Libraries
path = require('path');
json = require('../../package.json');
electron = require('electron');

// Owned libraries
bluetooth = require("./../../../../cougar_bluetooth_lib/build/Release/cougar_bluetooth_lib.node");
cougar = require("./../../../../cougar_lib/build/Release/obd_cougar.node");

exports.prepareMainWindow = function(app, onLoad)
{
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

    if (onLoad) {
    	onLoad();

      console.log(cougar);
      console.log(cougar.SerialPort());

      console.log("TEST");
      console.log(cougar.SerialPort().GetDevices());
      console.log("TEST END");
    }
  });

  window.on('closed', function() {
    window = null;
  });

  return window;
}