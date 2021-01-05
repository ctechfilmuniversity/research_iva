const IVA_WIDTH = 600, IVA_HEIGHT = 480;

const tones = ['E3', 'G3', 'A3', 'C4', 'D4', 'E4'];
const osc = new Tone.Oscillator('E3', 'sine').toDestination().start();

let cam, tonesPlayedGr, showUI = false, col, currentToneIndex = 0, slitIdx=0;

function setup() {
  createCheckbox('Show tones Played', false).changed(function() {
    showUI = this.checked();
  });
  createCanvas(IVA_WIDTH, IVA_HEIGHT);
  tonesPlayedGr = createGraphics(IVA_WIDTH, IVA_HEIGHT);

  cam = createCapture(VIDEO);
  cam.size(IVA_WIDTH, IVA_HEIGHT);
  cam.hide();
}

function draw() {
  col = cam.get(300, 0, 1, cam.height);
  calculateTone();
  image(col, slitIndex(), 0, 1, height);

  // tonesPlayed UI
  updateTonesPlayedGraphics();
  if (showUI) {
    image(tonesPlayedGr, 0, 0);
  }
}

function slitIndex() {
  if (slitIdx < width) {
    slitIdx++;
  } else {
      slitIdx = 0;
      // if (recordMode) {
      //     recorded = true;
      // }
  }
  return slitIdx;
}

function calculateTone() {
  col.loadPixels();

  let stepSize = IVA_HEIGHT / 6;

  let currentMeanBrightness = 0;
  let toneIndex = 0;

  for (let i = 0; i < IVA_HEIGHT; i += stepSize) {
    let accumulatedBrightness = 0;
    for (let inY = i; inY < i+stepSize; inY++) {
      let pxl = col.pixels.slice(inY, inY + 3);
      let pxBrightness = (pxl[0] + pxl[1] + pxl[2]) / 3;
      accumulatedBrightness += pxBrightness;
    }
    let meanBrightness = accumulatedBrightness / stepSize;

    if (meanBrightness > currentMeanBrightness) {
      currentMeanBrightness = meanBrightness;
      //currentAmp = ofMap(meanBrightness, 0, limit, 0, 1);
      toneIndex = i / stepSize;
    }
  }

  currentToneIndex = toneIndex;

  osc.frequency.rampTo(tones[currentToneIndex], 0);
}

function updateTonesPlayedGraphics() {
  if (slitIdx == 0) tonesPlayedGr.clear();

  tonesPlayedGr.stroke(100, 255, 100);
  tonesPlayedGr.strokeWeight(1);

  const toneHeight = IVA_HEIGHT / tones.length;
  tonesPlayedGr.line(slitIdx, toneHeight * currentToneIndex, slitIdx, toneHeight * (currentToneIndex + 1));
}