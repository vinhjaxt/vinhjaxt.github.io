var cacheName = 'tkb-v2'
var filesToCache = [
  '',
  'index.html',
  '/manifest.json'
]

self.addEventListener('install', function (e) {
  console.log('[ServiceWorker] Install')
  e.waitUntil(function () {
    caches.open(cacheName).then(function (cache) {
      console.log('[ServiceWorker] Caching app shell')
      return cache.addAll(filesToCache)
    })
  })
  return self.skipWaiting()
})

self.addEventListener('activate', function (e) {
  console.log('[ServiceWorker] Activate')
  e.waitUntil(caches.keys().then(function (keyList) {
    return Promise.all(keyList.map(function (key) {
      if (key !== cacheName) {
        console.log('[ServiceWorker] Removing old cache:', key)
        return caches.delete(key)
      }
    }))
  }))
  return self.clients.claim()
})

self.addEventListener('fetch', function (event) {
  console.log('Fetch event for:', event.request.url)
  event.respondWith(caches.match(event.request).then(function (response) {
    if (/\/data\.js/i.test(event.request.url)) {
      console.log('Force network request:', event.request.url)
      return new Promise(function (resolve, reject){
        fetch(event.request.clone()).then(function (r) {
          if (!r || r.status !== 200 || r.type !== 'basic') {
            postMessage('dataOK')
            resolve(r)
            return
          }
          var responseToCache = r.clone()
          caches.open(cacheName).then().then(function (cache) {
            cache.put(event.request, responseToCache)
          })
          postMessage('dataOK')
          resolve(r)
        }).catch(function (e){
          if(response){
            postMessage('dataOK')
            resolve(response)
          }else
            reject(e)
        })
      })
    }
    if (response) {
      console.log('Found ', event.request.url, ' in cache')
      return response
    }
    console.log('Network request:', event.request.url)
    return fetch(event.request.clone()).then(function (response) {
      if (!response || response.status !== 200 || response.type !== 'basic') {
        return response
      }
      var responseToCache = response.clone()
      caches.open(cacheName).then().then(function (cache) {
        cache.put(event.request, responseToCache)
      })
      return response
    })
  }).catch(function (error) {
    console.error('Fetch:', error)
    // return caches.match('/index.html')
  }))
})

function postMessage(data, extra){
  clients.matchAll().then(clients => {
    clients.forEach(client => {
      client.postMessage(data, extra)
    })
  })
}
