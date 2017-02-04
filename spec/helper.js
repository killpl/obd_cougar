var path = require('path');

module.exports = {
  appPath: function() {
    switch (process.platform) {
      case 'darwin':
        return path.join(__dirname, '..', '.tmp', 'OBDCougar-darwin-x64', 'OBDCougar.app', 'Contents', 'MacOS', 'OBDCougar');
      case 'linux':
        return path.join(__dirname, '..', '.tmp', 'OBDCougar-linux-x64', 'OBDCougar');
      default:
        throw 'Unsupported platform';
    }
  }
};
