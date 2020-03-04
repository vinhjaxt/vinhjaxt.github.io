importScripts('https://oss.sheetjs.com/sheetjs/shim.js')
importScripts('https://oss.sheetjs.com/sheetjs/xlsx.full.min.js')
postMessage({ t: 'ready' })

onmessage = e => {
  try {
    const workbook = XLSX.read(e.data, { type: 'binary' })
    let header
    const result = []
    workbook.SheetNames.forEach(sheetName => {
      let csv = XLSX.utils.sheet_to_csv(workbook.Sheets[sheetName])
      if (!csv.length) return
      csv = csv.replace(/^\s+|\s+$/g, '')
      const firstLineCharacter = csv.indexOf('\n')
      if (!~firstLineCharacter) {
        return
      }
      if (!header) {
        header = csv.substr(0, firstLineCharacter)
        postMessage({ t: 'h', d: header })
      }
      csv = csv.substr(firstLineCharacter + 1).replace(/^\s+/, '')
      result.push(csv)
    })
    postMessage({ t: 'ok', d: result.join("\n") })
  } catch (e) {
    postMessage({ t: 'e', d: e })
  }
}
