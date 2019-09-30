const puppeteer = require('puppeteer');

(async () => {
  const browser = await puppeteer.launch();
  const page = await browser.newPage();
  
  await page.goto('https://www.google.com/search?q=temperature&rlz=1C1CHBF_enVN853VN853&oq=tempera&aqs=chrome.0.69i59j69i57j69i61l2j69i60l2.1412j0j9&sourceid=chrome&ie=UTF-8');

  const stats = await page.evaluate( () => {
        var e =document.body.getElementsByClassName("wob_df wob_ds")[0];

        return {
                temp: document.getElementById('wob_ttm').innerText,
                high: e.childNodes[2].childNodes[0].childNodes[1].innerText,
                percip: document.getElementById('wob_pp').innerText
        }
  });
  console.log(stats);
  await browser.close();
})();
