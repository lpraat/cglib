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
	var vert2 = [[-1.0,-1.0,0.0], [1.0,-1.0,0.0], [1.0,1.0,0.0], [-1.0,1.0,0.0]];
	var ind2 = [0, 1, 2,  0, 2, 3];
	var color2 = [0.0, 0.0, 1.0];
	addMesh(vert2, ind2, color2);

	// Draws a Monopoly house
	var vert3 = [[-1.0,-1.0,0.0], [1.0,-1.0,0.0], [1.0,1.0,0.0], [-1.0,1.0,0.0], [0.0,1.5,0.0]];
	var ind3 = [0, 1, 2,  0, 2, 3,  3, 2, 4];
	var color3 = [0.0, 1.0, 0.0];
	addMesh(vert3, ind3, color3);

	// Draws a Cone
	var vert4 = [[0.0, 0.0, 0.0]];
	var ind4 = [];
	var color4 = [1.0, 1.0, 0.0];
	var slices4 = 6;
	for(i = 0; i < slices4; i++) {
		vert4[i+1] = [Math.cos(2*Math.PI / slices4 * i), Math.sin(2*Math.PI / slices4 * i), 0.0];
		ind4[3*i]   = 0;
		ind4[3*i+1] = i+1;
		ind4[3*i+2] = (i < slices4-1) ? i+2 : 1 ;
	}
	addMesh(vert4, ind4, color4);

	// Draws a Cylinder
	var vert5 = [[0.0, 0.0, 0.0]];
	var ind5 = [];
	var color5 = [1.0, 0.0, 1.0];
	var slices5 = 64;
	for(i = 0; i < slices5; i++) {
		vert5[i+1] = [Math.cos(2*Math.PI / slices5 * i), Math.sin(2*Math.PI / slices5 * i), 0.0];
		ind5[3*i]   = 0;
		ind5[3*i+1] = i+1;
		ind5[3*i+2] = (i < slices5-1) ? i+2 : 1 ;
	}
	addMesh(vert5, ind5, color5);

	// Draws a Sphere
	var vert6 = [[0.0, 0.0, 0.0]];
	var ind6 = [];
	var color6 = [0.0, 1.0, 1.0];
	var slices6 = 5;
	for(i = 0; i < slices6; i++) {
		vert6[i+1] = [Math.sin(2*Math.PI / slices6 * i), -Math.cos(2*Math.PI / slices6 * i), 0.0];
		vert6[i+slices6+1] = [2.6*Math.sin(2*Math.PI / slices6 * (i+0.5)), -2.6*Math.cos(2*Math.PI / slices6 * (i+0.5)), 0.0];
		ind6[6*i]   = 0;
		ind6[6*i+1] = i+1;
		ind6[6*i+2] = (i < slices6-1) ? i+2 : 1 ;
		ind6[6*i+4] = i+1;
		ind6[6*i+3] = (i < slices6-1) ? i+2 : 1 ;
		ind6[6*i+5] = slices6 + i + 1;
	}
	addMesh(vert6, ind6, color6);
}

