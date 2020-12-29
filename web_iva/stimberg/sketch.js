const IVA_WIDTH = 600, 
      IVA_HEIGHT = 480;

let cam, flow, currentFlow, stepSize, xSteps, ySteps;

const tones = ['A2', 'C4', 'G4', 'E4', 'A1', 'E5', 'B4', 'C5'];
let gainNodes = [];
let oscNodes = [];

//const osc = new Tone.Oscillator('E3', 'sine').toDestination().start();

function setup() {
  createCanvas(IVA_WIDTH, IVA_HEIGHT);

  // cam = createCapture(VIDEO);
  // cam.size(IVA_WIDTH, IVA_HEIGHT);
  // cam.hide();

  stepSize = 10;
  ySteps = IVA_WIDTH / stepSize;
  xSteps = IVA_HEIGHT / stepSize;

  // new approach async?
  flow = new oflow.WebCamFlow(null, stepSize);
  // Every time when optical flow is calculated
  // call the passed in callback:
  flow.onCalculated(function (direction) {
      // direction is an object which describes current flow:
      // direction.u, direction.v {floats} general flow vector
      // direction.zones {Array} is a collection of flowZones.
      // Each flow zone describes optical flow direction inside of it.
      // flowZone : {
      //  x, y // zone center
      //  u, v // vector of flow in the zone
      // }
      currentFlow = direction;
  });

  // Starts capturing the flow from webcamera:
  flow.startCapture();

  setupAudio();
}

function setupAudio() {
  tones.forEach(tone => {
    const gainNode = new Tone.Gain(0).toDestination();
    const osc = new Tone.Oscillator(tone, 'sine').connect(gainNode).start();

    gainNodes.push(gainNode);
    oscNodes.push(osc);
  });
}

function draw() {
  background(0);
  // col = cam.get(300, 0, 1, cam.height);
  // image(col, slitIndex(), 0, 1, height);

  drawFlowVectors();
}

function drawFlowVectors() {
  strokeWeight(1);
  if (currentFlow && currentFlow.zones) {
    currentFlow.zones.forEach(function(zone) {
      //stroke(map(zone.u, -stepSize, +stepSize, 0, 255), map(zone.v, -stepSize, +stepSize, 0, 255), 128);
      stroke(255);
      line(zone.x, zone.y, zone.x + zone.u, zone.y + zone.v);
    });
  }
}

function calculateGains() {
  let i = 0;
  let iTotal = xSteps * ySteps;
  
  const newAmp = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0];

  currentFlow.zones.forEach(zone => {
    // calc angle
    // const v1 = createVector(-1, 1);
    // const v2 = createVector(zone.u, zone.v);
    // let angle = v1.angleBetween(v2);

    translate(zone.x, zone.y);
    let angle = atan2(zone.v, zone.u);

    if (zone.u == 0 || zone.v == 0) {
      console.log("Skpppe");
      return;
    }

    console.log("?==0====");
    //console.log(angle);
    console.log(degrees(angle));

    // if (angle < 90) {
    //   newAmp[0] += 1.0 / iTotal;
    //   newAmp[4] += offset.length() / iTotal * 0.25;
    // } else if (angle < 180) {
    //   newAmp[1] += 1.0 / iTotal;
    //   newAmp[5] += offset.length() / iTotal * 0.25;
    // } else if (angle < 270) {
    //   newAmp[2] += 1.0 / iTotal;
    //   newAmp[6] += offset.length() / iTotal * 0.25;
    // } else  {
    //   newAmp[3] += 1.0 / iTotal;
    //   newAmp[7] += offset.length() / iTotal * 0.25;
    // }
  });
      
      
        
      


  // for (let i = 0; i < gainNodes.length; i++) {
  //   gainNodes[i].rampTo(newAmp[i], 1);
  // }
}