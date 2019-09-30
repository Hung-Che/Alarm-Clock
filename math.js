const puppeteer = require('puppeteer');

(async () => {
  const browser = await puppeteer.launch();
  const page = await browser.newPage();
  
  await page.goto('https://www.mathopolis.com/questions/day.php?y=3');

  const stats = await page.evaluate( () => {

        return {
                question: document.body.getElementsByClassName("question")[0].innerText,
                A: document.body.getElementsByClassName("answer")[0].childNodes[1].innerText,
                B: document.body.getElementsByClassName("answer")[1].childNodes[1].innerText,
                C: document.body.getElementsByClassName("answer")[2].childNodes[1].innerText,
                D: document.body.getElementsByClassName("answer")[3].childNodes[1].innerText
        }
  });
  console.log(stats);
  await browser.close();
})();
