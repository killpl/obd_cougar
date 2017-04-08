var electron, path, json;

// Logs window - attachable to main window bottom
var logsWindow = null;

// Logs node - will be moved between windows. Attached to main window if 
// logsWindow is null.
var logsNode = null;

// Libraries
path = require('path');
json = require('../../package.json');
electron = require('electron');

// Import windows creation logic
prepareMainWindow = require('./mainWindow').prepareMainWindow


electron.app.on('ready', function() {

  var _onWindowLoaded = function()
  {
    var _logFunction = function(timestamp, level, str) {
        console.log(str);
        window.webContents.send('log_line',
          { 
            timestamp: timestamp,
            level:     level,
            str:       str,
          }
        );
    };

    console.log(bluetooth);
    console.log(cougar);

    bluetooth.SetLogCallback(_logFunction);
    cougar.SetLogCallback(_logFunction);

    b = new bluetooth.Cougar_Bluetooth();
    console.log(b.GetStatus());
    b.StartScan("", _logFunction);

  };

  var window = prepareMainWindow(electron.app, _onWindowLoaded);
});
