
// Libraries
path = require('path');
json = require('../../package.json');
electron = require('electron');


exports.prepareLogsWindow = function(onLoad)
{
  var window;

  window = new electron.BrowserWindow({
    title: "Logs",
    width: 400,
    height: 300
  });

  window.loadURL('file://' + path.join(__dirname, '..', '..') + '/logsWindow.html');

  window.webContents.on('did-finish-load', function(){
    if (onLoad) {
    	onLoad();
    }
  });

  window.on('closed', function() {
    window = null;

    // Re-attach

  });

  return window;
}

exports.prepareLogsNode =  function ()
{

}