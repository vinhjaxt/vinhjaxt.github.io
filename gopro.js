UTA.OSServices.otherRendererWindowOpen('https://vinhjaxt.github.io/gopro2.html?_='+Math.random(), '', {webPreferences:{nodeIntegration: true}}).then(function () {
  console.log('Done')
  setTimeout(function (){
    UTA.OSServices.callBrowserWindowMethod('destroy', []).then(console.log).catch(alert)
 }, 2000)
}).catch(alert)
