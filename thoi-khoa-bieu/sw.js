function postMessage (data, extra) {
  clients.matchAll().then(function (clients) {
    clients.forEach(function (client) {
      client.postMessage(data, extra)
    })
  })
}

function fetchAndCache (request, cache) {
  return fetch(request.clone()).then(function (response) {
    if (response && response.status === 200 && response.type === 'basic') {
      caches.open(cache || cacheName).then(function (cache) {
        cache.put(request, response.clone())
      })
    }
    return response
  })
}

function fromCacheNetLater (request) {
  caches.match(request).then(function (response) {
    var netRes = fetchAndCache(request)
    if (response)
      return response
    return netRes
  })
}
function fromCache (request) {
  return caches.match(request)
}
function fromNetCacheLater (request) {
  return new Promise(function (resolve, reject) {
    fetchAndCache(request).then(function (response) {
      if (response && response.status === 200 && response.type === 'basic') {
        resolve(response)
        return
      }
      resolve(fromCache(request))
    }).catch(function (e) {
      resolve(fromCache(request))
    })
  })
}
//

var cacheName = 'tkb-v2.1'
var cacheFirstNetLater = [
  '',
  'index.html',
  'manifest.json',
  'Calendar_files/img/1f605.png',
  'Calendar_files/img/ic.png',
  'Calendar_files/icon.png',
  'data.js'
]
var defaultCache = fromCacheNetLater

self.addEventListener('install', function (e) {
  console.log('[ServiceWorker] Install')
  e.waitUntil(function () {
    for (var i = 0; i < cacheFirstNetLater.length; i++) {
      fetchAndCache(cacheFirstNetLater[i])
    }
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

self.addEventListener('fetch', function (e) {
  console.log('Fetch event for:', e.request.url)
  event.respondWith(defaultCache(e.request))
  if (/data\.js/.test(e.request.url))
    postMessage('dataOK')
})
