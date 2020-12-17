
const IVA_WIDTH = 600, 
      IVA_HEIGHT = 480;

let cam, stepSize, xSteps, ySteps;

// const tones = ["E3", "G3", "A3", "C4", "D4", "E4"];

//const osc = new Tone.Oscillator("E3", "sine").toDestination().start();

function setup() {
  createCanvas(IVA_WIDTH, IVA_HEIGHT);

  cam = createCapture(VIDEO);
  cam.size(IVA_WIDTH, IVA_HEIGHT);
  cam.hide();

  stepSize = 10;
  ySteps = cam.width / stepSize;
  xSteps = cam.height / stepSize;


}

function draw() {
  // col = cam.get(300, 0, 1, cam.height);
  // image(col, slitIndex(), 0, 1, height);
}
