const IVA_WIDTH = 600, 
      IVA_HEIGHT = 480;

let cam, flow, currentFlow, stepSize, xSteps, ySteps;

const tones = ['A2', 'C4', 'G4', 'E4', 'A1', 'E5', 'B4', 'C5'];
let gainNodes = [];
let oscNodes = [];

function setup() {
  createCanvas(IVA_WIDTH, IVA_HEIGHT);

  stepSize = 10;
  ySteps = IVA_WIDTH / stepSize;
  xSteps = IVA_HEIGHT / stepSize;

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

  flowVectorOutput();
}

function flowVectorOutput() {
  strokeWeight(1);
  if (currentFlow && currentFlow.zones) {
    // tone generation
    const newAmp = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0];
    const iTotal = xSteps * ySteps;

    currentFlow.zones.forEach(zone => {
      // drawing the flow vectors
      //stroke(map(zone.u, -stepSize, +stepSize, 0, 255), map(zone.v, -stepSize, +stepSize, 0, 255), 128);
      stroke(255);
      line(zone.x, zone.y, zone.x + zone.u, zone.y + zone.v);

      
      // generating sound
      if (zone.u == 0 || zone.v == 0) return;

      // calc angle
      translate(zone.x, zone.y);
      let angle = atan2(zone.v, zone.u); // radians y, x
      let offset = createVector(zone.u, zone.v).mag().toFixed(2);
      translate(- zone.x, - zone.y);

      angle = degrees(angle).toFixed(0) // to degrees
      if (angle < 0) angle = 180 - angle; // having degrees until 360

      if (angle < 90) {
        newAmp[0] += 1.0 / iTotal;
        newAmp[4] += offset / iTotal * 0.25;
      } else if (angle < 180) {
        newAmp[1] += 1.0 / iTotal;
        newAmp[5] += offset / iTotal * 0.25;
      } else if (angle < 270) {
        newAmp[2] += 1.0 / iTotal;
        newAmp[6] += offset / iTotal * 0.25;
      } else  {
        newAmp[3] += 1.0 / iTotal;
        newAmp[7] += offset / iTotal * 0.25;
      }
    });

    for (let i = 0; i < 8; i++) {
      //amps[i].set(newAmp[i]);
      gainNodes[i].gain.rampTo(newAmp[i], .2); // change gain within 1 second
    }
  }
}
