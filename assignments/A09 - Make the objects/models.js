function buildGeometry() {
	var i;

	// Draws a pyramid
  var vert1 = [[0, 1, -0.5],
               [-0.5, 0, -1],
               [-0.5, 0, 0],

               [0, 1, -0.5],
               [-0.5, 0, 0],
               [0.5, 0, 0],

               [0, 1, -0.5],
               [0.5, 0, 0],
               [0.5, 0, -1],

               [0, 1, -0.5],
               [0.5, 0, -1],
               [-0.5, 0, -1],

               [0.5, 0, 0],
               [-0.5, 0, 0],
               [-0.5, 0, -1],

               [0.5, 0, 0],
               [-0.5, 0, -1],
               [0.5, 0, -1]];

  var ind1 = [];
  for (let i = 0; i < vert1.length; i++) {
    ind1.push(i);
  }

	var color1 = [1.0, 0.0, 0.0];
	addMesh(vert1, ind1, color1);

	// Draws a cube
	var vert2 = [
    // front face
    [-0.5, 0, -0.5],
    [0.5, 0, -0.5],
    [0.5, 1, -0.5],

    [-0.5, 0, -0.5],
    [0.5, 1, -0.5],
    [-0.5, 1, -0.5],

    // left face
    [-0.5, 0, -1.5],
    [-0.5, 0, -0.5],
    [-0.5, 1, -0.5],

    [-0.5, 1, -1.5],
    [-0.5, 0, -1.5],
    [-0.5, 1, -0.5],

    // right face
    [0.5, 0, -0.5],
    [0.5, 0, -1.5],
    [0.5, 1, -1.5],

    [0.5, 0, -0.5],
    [0.5, 1, -1.5],
    [0.5, 1, -0.5],

    // back face
    [-0.5, 0, -1.5],
    [0.5, 1, -1.5],
    [0.5, 0, -1.5],

    [-0.5, 0, -1.5],
    [-0.5, 1, -1.5],
    [0.5, 1, -1.5],


    // top face
    [-0.5, 1, -0.5],
    [0.5, 1, -0.5],
    [-0.5, 1, -1.5],

    [-0.5, 1, -1.5],
    [0.5, 1, -0.5],
    [0.5, 1, -1.5],

    // bottom face
    [0.5, 0, -0.5],
    [-0.5, 0, -0.5],
    [0.5, 0, -1.5],

    [-0.5, 0, -0.5],
    [-0.5, 0, -1.5],
    [0.5, 0, -1.5],
  ];

  var ind2 = [];

  for (let i = 0; i < vert2.length; i++) {
    ind2.push(i);
  }

	var color2 = [0.0, 0.0, 1.0];
	addMesh(vert2, ind2, color2);

	// Draws a Monopoly house
	var vert3 = [
     // Parallelepiped
     // front face
     [-0.5, 0, -0.5],
     [0.5, 0, -0.5],
     [0.5, 1, -0.5],

     [-0.5, 0, -0.5],
     [0.5, 1, -0.5],
     [-0.5, 1, -0.5],

     // left face
     [-0.5, 0, -2.0],
     [-0.5, 0, -0.5],
     [-0.5, 1, -0.5],

     [-0.5, 1, -2.0],
     [-0.5, 0, -2.0],
     [-0.5, 1, -0.5],

     // right face
     [0.5, 0, -0.5],
     [0.5, 0, -2.0],
     [0.5, 1, -2.0],

     [0.5, 0, -0.5],
     [0.5, 1, -2.0],
     [0.5, 1, -0.5],

     // back face
     [-0.5, 0, -2.0],
     [0.5, 1, -2.0],
     [0.5, 0, -2.0],

     [-0.5, 0, -2.0],
     [-0.5, 1, -2.0],
     [0.5, 1, -2.0],


     // top face
     [-0.5, 1, -0.5],
     [0.5, 1, -0.5],
     [-0.5, 1, -2.0],

     [-0.5, 1, -2.0],
     [0.5, 1, -0.5],
     [0.5, 1, -2.0],

     // bottom face
     [0.5, 0, -0.5],
     [-0.5, 0, -0.5],
     [0.5, 0, -2.0],

     [-0.5, 0, -0.5],
     [-0.5, 0, -2.0],
     [0.5, 0, -2.0],

     // Top Pyramid
     // front face
     [-0.5, 1, -0.5],
     [0.5, 1, -0.5],
     [0, 1.5, -0.5],

     // right face
     [0.5, 1, -0.5],
     [0.5, 1, -2],
     [0, 1.5, -0.5],

     [0.5, 1, -2],
     [0, 1.5, -2],
     [0, 1.5, -0.5],


     // left face
     [-0.5, 1, -2],
     [-0.5, 1, -0.5],
     [0, 1.5, -0.5],

     [0, 1.5, -2],
     [-0.5, 1, -2],
     [0, 1.5, -0.5],

     // back face
     [0.5, 1, -2],
     [-0.5, 1, -2],
     [0, 1.5, -2],
  ];
  var ind3 = [];

  for (let i = 0; i < vert3.length; i++) {
    ind3.push(i);
  }

	var color3 = [0.0, 1.0, 0.0];
	addMesh(vert3, ind3, color3);

	// Draws a Cone
  var vert4 = [];
  let basePoints4 = [];
	var ind4 = [];
	var color4 = [1.0, 1.0, 0.0];
	var slices4 = 64;
	for(i = 0; i < slices4; i++) {
		basePoints4.push([Math.cos(2*Math.PI / slices4 * i), 0.0, -Math.sin(2*Math.PI / slices4 * i)]);
  }

  for (let i = 0; i < basePoints4.length; i++) {
    let j = i+1;
    if (j == basePoints4.length) {
      j = 0;
    }

    // Base
    vert4.push(basePoints4[j]);
    vert4.push(basePoints4[i]);
    vert4.push([0.0, 0.0, 0.0]);

    // Top connection
    vert4.push(basePoints4[i]);
    vert4.push(basePoints4[j]);
    vert4.push([0.0, 2.0, 0.0]);
  }

  for (let i = 0; i < vert4.length; i++) {
    ind4.push(i);
  }

	addMesh(vert4, ind4, color4);

	// Draws a Cylinder
	var vert5 = [];
	var ind5 = [];
	var color5 = [1.0, 0.0, 1.0];
  var slices5 = 6;
  let basePoints5 = [];
  let topPoints5 = [];
	for(let i = 0; i < slices5; i++) {
    basePoints5.push([Math.cos(2*Math.PI / slices5 * i), 0.0, -Math.sin(2*Math.PI / slices5 * i)]);
    topPoints5.push([Math.cos(2*Math.PI / slices5 * i), 2.0, -Math.sin(2*Math.PI / slices5 * i)]);
  }

  for (let i = 0; i < basePoints5.length; i++) {
    let j = i+1;
    if (j == basePoints5.length) {
      j = 0;
    }

    // Base face
    vert5.push(basePoints5[j]);
    vert5.push(basePoints5[i]);
    vert5.push([0.0, 0.0, 0.0]);

    // Top face
    vert5.push(topPoints5[i]);
    vert5.push(topPoints5[j]);
    vert5.push([0.0, 2.0, 0.0]);

    // Front face
    vert5.push(basePoints5[i]);
    vert5.push(basePoints5[j]);
    vert5.push(topPoints5[j]);

    vert5.push(basePoints5[i]);
    vert5.push(topPoints5[j]);
    vert5.push(topPoints5[i]);
  }

  for (let i = 0; i < vert5.length; i++) {
    ind5.push(i);
  }

	addMesh(vert5, ind5, color5);

  // Draws a Sphere
  // Sphere is divided by sectors(horizontally) and stacks(vertically)
  // Point (x, y, z) on sphere can be computed given sector angle theta and stack angle phi as
    // x = (r * cos(phi)) * cos(theta)
    // y = (r * cos(phi)) * sin(theta)
    // z = r * sin(phi)

  // Where theta = 2*Pi*(sectorStep / sectorCount) [from 0 to 360 degree]
  // phi = pi/2 - pi * (stackStep / stackCount) [from -90 to 90]

	var vert6 = [];
	var ind6 = [];
	var color6 = [0.0, 1.0, 1.0];
  const radius = 1.0;

  const sectorCount = 64;
  const sectorStep = 2 * Math.PI / sectorCount;

  const stackCount = 64;
  const stackStep = Math.PI / stackCount;

  for (let i = 0; i <= stackCount; i++) {
    let stackAngle = Math.PI/2 - i * stackStep; // starting form pi/2 to -pi/2

    for (let j = 0; j <= sectorCount; j++) {
      let sectorAngle = j * sectorStep;

      vert6.push([
        radius * Math.cos(stackAngle) * Math.cos(sectorAngle),
        radius * Math.cos(stackAngle) * Math.sin(sectorAngle),
        radius * Math.sin(stackAngle)
      ]);
    }
  }

  for (let i = 0; i < stackCount; i++) {

    let k1 = i * (sectorCount + 1); // beginning of current stack
    let k2 = k1 + sectorCount + 1; // beginning of next stack

    for (let j = 0; j < sectorCount; j++, k1++, k2++) {
      // 2 triangles per sector excluding first and last stacks
      if (i !== 0) {
        ind6.push(k1);
        ind6.push(k2);
        ind6.push(k1+1);
      }

      if (i !== (stackCount-1)) {
        ind6.push(k1 + 1);
        ind6.push(k2);
        ind6.push(k2 + 1);
      }
    }
  }


	addMesh(vert6, ind6, color6);
}

