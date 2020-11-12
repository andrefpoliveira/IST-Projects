var scene, renderer;

var orthographicCamera, perspectiveCamera, currentCamera, changeToCamera;

var spot1, spot2, spot3, directionalLight;

var toggleIluminationCalcs = false, calcsEnabled = true, changeShading = false, phongEnabled = true;

var meshMaterials = [];

var cybertruck, palanque;

var clock;

const FACTOR = 3.5;

function createAllMaterials(colorToSet) {
    'use strict'
    var basicMaterial = new THREE.MeshBasicMaterial({ color: colorToSet, wireframe: false });
    var lambertMaterial = new THREE.MeshLambertMaterial({ color: colorToSet, wireframe: false });
    var phongMaterial = new THREE.MeshPhongMaterial({ color: colorToSet, wireframe: false });
    return [basicMaterial, lambertMaterial, phongMaterial];
}

function createPalanque() {
    'use strict';
    palanque = new THREE.Object3D()   
    palanque.data = {rotateCW: false, rotateCCW: false};

    var materials = createAllMaterials(0x808080);
    var geometry = new THREE.CylinderGeometry(130, 150, 15, 32);
    geometry.translate(0, 7.5, 0);

    var mesh = new THREE.Mesh(geometry, materials[2]);
    materials.push(mesh);
    meshMaterials.push(materials); 
    palanque.add(mesh);
    return palanque;
}

function createCylinder(obj, x, y, z) {
    'use strict';
    var materials = createAllMaterials(0x333333);

    var geometry = new THREE.CylinderGeometry(19, 19, 12, 64);
    geometry.rotateZ(Math.PI/2);
    geometry.translate(x, y, z);
    
    var mesh = new THREE.Mesh(geometry, materials[2]);
    materials.push(mesh);
    meshMaterials.push(materials);  
    obj.add(mesh);
}

function createBase(obj, x, y, z, l, c, h, rotX, rotY, rotZ, color) {
    'use strict';
    var materials = createAllMaterials(color);
    var geometry = new THREE.BoxGeometry(l, c, h);
    geometry.rotateX(rotX).rotateY(rotY).rotateZ(rotZ);

    var mesh = new THREE.Mesh(geometry, materials[2]);
    mesh.position.set(x, y, z);
    materials.push(mesh);
    meshMaterials.push(materials);
    obj.add(mesh);
}

function createCube(obj, x, y, z, l, c, h, rotX, rotY, rotZ, color) {
    'use strict';
    var materials = createAllMaterials(color);
    var geometry = new THREE.BoxGeometry(l, c, h);
    geometry.rotateX(rotX).rotateY(rotY).rotateZ(rotZ);

    var mesh = new THREE.Mesh(geometry, materials[2]);
    mesh.position.set(x, y, z);

    materials.push(mesh);
    meshMaterials.push(materials);
    obj.add(mesh);
}

function createCybertruck() {
    'use strict'
    cybertruck = new THREE.Object3D();

    createCylinder(cybertruck, 46.5, 34, 76); // Frente esquerda
    createCylinder(cybertruck, 46.5, 34, -76); // Tras esquerda
    createCylinder(cybertruck, -46.5, 34, 76); // Frente direita
    createCylinder(cybertruck, -46.5, 34, -76); // Tras direita
    createCube(cybertruck, 0, 34, 0, 81, 5, 152, 0, 0, 0, 0x5A5A5A);

    var part1 = createFace();
    cybertruck.add(part1);

    var headLights = createHeadLights();
    cybertruck.add(headLights);

    var breakLights = createBreakLights();
    cybertruck.add(breakLights);

    var windows = createWindow();
    cybertruck.add(windows);
    return cybertruck;
}

function createHeadLights() {
    'use strict'
    var materials = createAllMaterials(0x8FFFF1);
    var geometry = new THREE.Geometry();

    //farol esquerda
    geometry.vertices.push( new THREE.Vector3(-30, 55, 120)); // 0
    geometry.vertices.push( new THREE.Vector3(-30, 45, 120)); // 1
    geometry.vertices.push( new THREE.Vector3(-20, 55, 120)); // 2 
    geometry.vertices.push( new THREE.Vector3(-20, 45, 120)); // 3

    //farol direito
    geometry.vertices.push( new THREE.Vector3(30, 55, 120)); // 4
    geometry.vertices.push( new THREE.Vector3(30, 45, 120)); // 5 
    geometry.vertices.push( new THREE.Vector3(20, 55, 120)); // 6
    geometry.vertices.push( new THREE.Vector3(20, 45, 120)); // 7

    // Farol Esquerda
    const face1 = new THREE.Face3(0, 1, 2);
    const face2 = new THREE.Face3(3, 2, 1);

    // Farol Direita
    const face3 = new THREE.Face3(6, 5, 4);
    const face4 = new THREE.Face3(5, 6, 7);
    
    geometry.faces.push(face1, face2, face3, face4);
    geometry.computeFaceNormals();
    var mesh = new THREE.Mesh(geometry, materials[2]);
    materials.push(mesh);
    meshMaterials.push(materials);
    return mesh;
}

function createBreakLights() {
    'use strict';
    var materials = createAllMaterials(0xFF0000);
    var geometry = new THREE.Geometry();

    geometry.vertices.push( new THREE.Vector3(-46.5, 75, -130)); // 0
    geometry.vertices.push( new THREE.Vector3(46.5, 75, -130)); // 1

    geometry.vertices.push( new THREE.Vector3(-46.5, 70, -130)); // 2
    geometry.vertices.push( new THREE.Vector3(46.5, 70, -130)); // 3

    const face2 = new THREE.Face3(0, 1, 3);
    const face1 = new THREE.Face3(3, 2, 0);

    geometry.faces.push(face1, face2);

    geometry.computeFaceNormals();
    var mesh = new THREE.Mesh(geometry, materials[2]);
    materials.push(mesh);
    meshMaterials.push(materials);
    return mesh;
}

function createWindow() {
    'use strict';
    var materials = createAllMaterials(0x000000);
    var geometry = new THREE.Geometry();

    geometry.vertices.push( new THREE.Vector3(-40, 66.7, 80)); // 0
    geometry.vertices.push( new THREE.Vector3(40, 66.7, 80)); // 1
    geometry.vertices.push( new THREE.Vector3(-40, 86.7, 20)); // 2
    geometry.vertices.push( new THREE.Vector3(40, 86.7, 20)); // 3

    geometry.vertices.push( new THREE.Vector3(46.5, 75, -50)); // 4
    geometry.vertices.push( new THREE.Vector3(46.5, 63, 75)); // 5
    geometry.vertices.push( new THREE.Vector3(46.5, 85, 10)); // 6

    geometry.vertices.push( new THREE.Vector3(-46.5, 75, -50)); // 7
    geometry.vertices.push( new THREE.Vector3(-46.5, 63, 75)); // 8
    geometry.vertices.push( new THREE.Vector3(-46.5, 85, 10)); // 9

    // Capô
    const face1 = new THREE.Face3(0, 1, 2);
    const face2 = new THREE.Face3(3, 2, 1);
    
    // Left Side
    const face3 = new THREE.Face3(6,5,4);

    // Right Side
    const face4 = new THREE.Face3(7,8,9);

    geometry.faces.push(face1, face2, face3, face4);

    geometry.computeFaceNormals();

    var mesh = new THREE.Mesh(geometry, materials[2]);
    materials.push(mesh);
    meshMaterials.push(materials);
    scene.add(mesh);
    return mesh;
}

function createFace() {
    'use strict';
    var materials = createAllMaterials(0x5A5A5A);

    var geometry = new THREE.Geometry();
    geometry.vertices.push( new THREE.Vector3(-46.5, 60, 100 )); // 0
    geometry.vertices.push( new THREE.Vector3(-30, 55, 120)); // 1
    geometry.vertices.push( new THREE.Vector3(30, 55, 120)); // 2
    geometry.vertices.push( new THREE.Vector3(46.5, 60, 100)); // 3
    geometry.vertices.push( new THREE.Vector3(46.5, 90, 10)); // 4
    geometry.vertices.push( new THREE.Vector3(-46.5, 90, 10)); // 5

    geometry.vertices.push( new THREE.Vector3(-46.5, 75, -130)); // 6
    geometry.vertices.push( new THREE.Vector3(46.5, 75, -130)); // 7
    
    //farol esquerda
    geometry.vertices.push( new THREE.Vector3(-30, 55, 120)); // 8
    geometry.vertices.push( new THREE.Vector3(-30, 45, 120)); // 9
    geometry.vertices.push( new THREE.Vector3(-20, 55, 120)); // 10 
    geometry.vertices.push( new THREE.Vector3(-20, 45, 120)); // 11

    //farol direito
    geometry.vertices.push( new THREE.Vector3(30, 55, 120)); // 12
    geometry.vertices.push( new THREE.Vector3(30, 45, 120)); // 13 
    geometry.vertices.push( new THREE.Vector3(20, 55, 120)); // 14
    geometry.vertices.push( new THREE.Vector3(20, 45, 120)); // 15

    // Buracos frente
    geometry.vertices.push( new THREE.Vector3(-40, 66.7, 80)); // 16
    geometry.vertices.push( new THREE.Vector3(40, 66.7, 80)); // 17
    geometry.vertices.push( new THREE.Vector3(-40, 86.7, 20)); // 18
    geometry.vertices.push( new THREE.Vector3(40, 86.7, 20)); // 19

    // Left side
    geometry.vertices.push( new THREE.Vector3(46.5, 30, 100)); // 20
    geometry.vertices.push( new THREE.Vector3(46.5, 30, -130)); // 21
    geometry.vertices.push( new THREE.Vector3(46.5, 75, -50)); // 22
    geometry.vertices.push( new THREE.Vector3(46.5, 63, 75)); // 23
    geometry.vertices.push( new THREE.Vector3(46.5, 85, 10)); // 24

    // Right side
    geometry.vertices.push( new THREE.Vector3(-46.5, 30, 100)); // 25
    geometry.vertices.push( new THREE.Vector3(-46.5, 30, -130)); // 26
    geometry.vertices.push( new THREE.Vector3(-46.5, 75, -50)); // 27
    geometry.vertices.push( new THREE.Vector3(-46.5, 63, 75)); // 28
    geometry.vertices.push( new THREE.Vector3(-46.5, 85, 10)); // 29

    geometry.vertices.push( new THREE.Vector3(-30, 30, 120)); // 30
    geometry.vertices.push( new THREE.Vector3(30, 30, 120)); // 31
    
    geometry.vertices.push( new THREE.Vector3(-46.5, 70, -130)); // 32
    geometry.vertices.push( new THREE.Vector3(46.5, 70, -130)); // 33
    

    // Capô
    const face1 = new THREE.Face3(0, 1, 2);
    const face2 = new THREE.Face3(0, 2, 3);
    const face3 = new THREE.Face3(17, 16, 0);
    const face4 = new THREE.Face3(0, 3, 17);
    const face5 = new THREE.Face3(16, 5, 0);
    const face6 = new THREE.Face3(5, 16, 18);
    const face7 = new THREE.Face3(5, 18, 19);
    const face8 = new THREE.Face3(4, 5, 19);
    const face9 = new THREE.Face3(19, 17, 4);
    const face10 = new THREE.Face3(3, 4, 17);

    // Tejadilho
    const face11= new THREE.Face3(7, 6, 5);
    const face12 = new THREE.Face3(7, 5, 4);

    // Porta esquerda
    const face13 = new THREE.Face3(7, 20, 21);
    const face14 = new THREE.Face3(7, 22, 20);
    const face15 = new THREE.Face3(20, 22, 23);
    const face16 = new THREE.Face3(3, 20, 23);
    const face17 = new THREE.Face3(22, 7, 4);
    const face18 = new THREE.Face3(24, 22, 4);
    const face19 = new THREE.Face3(23, 4, 3);
    const face20 = new THREE.Face3(4, 23, 24);

    // Porta direita
    const face21 = new THREE.Face3(26, 25, 6);
    const face22 = new THREE.Face3(25, 27, 6);
    const face23 = new THREE.Face3(28, 27, 25);
    const face24 = new THREE.Face3(28, 25, 0);
    const face25 = new THREE.Face3(5, 6, 27);
    const face26 = new THREE.Face3(5, 27, 29);
    const face27 = new THREE.Face3(0, 5, 28);
    const face28 = new THREE.Face3(29, 28, 5);
    
    // Front part
    const face29 = new THREE.Face3(14, 10, 11);
    const face30 = new THREE.Face3(15, 14, 11);

    // Back part
    const face31 = new THREE.Face3(21, 32, 33);
    const face32 = new THREE.Face3(26, 32, 21);

    // Para choques
    const face33 = new THREE.Face3(20, 3, 2);
    const face34 = new THREE.Face3(31, 20, 2);

    const face35 = new THREE.Face3(25, 1, 0);
    const face36 = new THREE.Face3(1, 25, 30);

    const face37 = new THREE.Face3(30, 13, 9);
    const face38 = new THREE.Face3(13, 30, 31);
    
    geometry.faces.push(face1, face2, face3, face4, face5, face6, face7, face8, face9, face10,
                        face11, face12, face13, face14, face15, face16,face17, face18, face19, face20,
                        face21, face22, face23, face24, face25, face26, face27, face28, face29, face30,
                        face31, face32, face33, face34, face35, face36, face37, face38);
                        
    geometry.computeFaceNormals();
    var mesh = new THREE.Mesh(geometry, materials[2]);
    materials.push(mesh);
    meshMaterials.push(materials);
    scene.add(mesh);
    return mesh;
}

function createDirectionalLight() {
    'use strict';
    directionalLight = new THREE.DirectionalLight();
    directionalLight.target = cybertruck;
    directionalLight.translateX(25).translateZ(25);
    scene.add(directionalLight);
}

function createSpotLight(x, y, z) {
    'use strict';
    var light = new THREE.SpotLight( 0xffffff );
    light.position.set( x, y, z );

    light.castShadow = true;

    light.shadow.mapSize.width = window.innerWidth;
    light.shadow.mapSize.height = window.innerHeight;

    light.shadow.camera.near = 1;
    light.shadow.camera.far = 1000;
    light.shadow.camera.fov = 70;

    scene.add(light);
    return light
}

function createLightStructure(x, y, z, rotX, rotY, rotZ) {
    'use strict';
    const geometry = new THREE.SphereGeometry(9, 32, 32);
    const material = new THREE.MeshBasicMaterial( {color: 0xffffff} );
    const sphere = new THREE.Mesh(geometry, material);
    sphere.translateX(x).translateY(y).translateZ(z);

    const geometry1 = new THREE.ConeGeometry(10, 20, 32);
    const material1 = new THREE.MeshBasicMaterial( {color: 0xffff00} );
    const cone = new THREE.Mesh(geometry1, material1);
    cone.translateX(x).translateY(y).translateZ(z);
    cone.rotateX(Math.PI/2);
    cone.lookAt(new THREE.Vector3(0, 300, 0));
    
    scene.add(cone);
    scene.add(sphere);
}

function createShowcase() {
    'use strict';
    var base = new THREE.Object3D();
    createBase(base, 0, -1, 0, 2000, 2, 2000, 0, 0, 0, 0x222222);

    createPalanque();
    var cybertruck = createCybertruck();

    palanque.add(cybertruck);

    spot1 = createSpotLight(0, 150, 150);
    spot2 = createSpotLight(-150, 150, -150);
    spot3 = createSpotLight(150, 150, -150);

    createDirectionalLight();

    createLightStructure(0, 150, 150, 0, 0, 0);
    createLightStructure(-150, 150, -150, 0, 0, 0);
    createLightStructure(150, 150, -150, 0, 0, 0);

    scene.add(base);
    scene.add(palanque);
}

function createScene() {
    'use strict';
    scene = new THREE.Scene();
    createShowcase();
}

function createCameras() {
    'use strict';
    perspectiveCamera = createPerspectiveCamera(250, 150, 250);
    orthographicCamera = createOrthographicCamera(300, 0, 0);
    changeToCamera = currentCamera = perspectiveCamera;
}

function createOrthographicCamera(x, y, z) {
    'use strict';
    var orthographic = new THREE.OrthographicCamera(window.innerWidth / -FACTOR, window.innerWidth / FACTOR, window.innerHeight / FACTOR, window.innerHeight / -FACTOR, 1, 1000 );
    orthographic.position.x = x;
    orthographic.position.y = y;
    orthographic.position.z = z;
    orthographic.lookAt(scene.position);
    return orthographic;
}

function createPerspectiveCamera(x, y, z) {
    'use strict';
    var camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 1,1000);
    camera.position.x = x;
    camera.position.y = y;
    camera.position.z = z;
    camera.lookAt(scene.position);
    return camera;
}

function onResize() {
    'use strict';
    renderer.setSize(window.innerWidth, window.innerHeight);
    
    if (window.innerHeight > 0 && window.innerWidth > 0) {
        if (currentCamera === orthographicCamera) {
            currentCamera.left = window.innerWidth / -FACTOR;
            currentCamera.right = window.innerWidth / FACTOR;
            currentCamera.top = window.innerHeight / FACTOR;
            currentCamera.bottom = window.innerHeight / -FACTOR;
        } else {        
            currentCamera.aspect = window.innerWidth / window.innerHeight;
        }
        currentCamera.updateProjectionMatrix();
    }
}

function onKeyDown(e) {
    'use strict';
    switch (e.keyCode) {
        // Q, q Switching on/off directional light
        case 81:
        case 113: {
            directionalLight.visible = !directionalLight.visible;
            break;
        }
        // W, w Toggle materials changing
        case 87:
        case 119: {
            toggleIluminationCalcs = true;
            break;
        }
        // E, e Change shadings
        case 69:
        case 101: {
            changeShading = true;
            break;
        }
        // 1, Switch on/off spotlight 1
        case 49: {
            spot1.visible = !spot1.visible;
            break;
        }
        // 2, Switch on/off spotlight 2
        case 50: {
            spot2.visible = !spot2.visible;
            break;
        }
        // 3, Switch on/off spotlight 3
        case 51: {
            spot3.visible = !spot3.visible;
            break;
        }
        // 4, Changing to perspective camera
        case 52: {
            changeToCamera = perspectiveCamera;
            break;
        }
        // 5, Changing to orthographic camera
        case 53: {
            changeToCamera = orthographicCamera;
            break;
        }
        // Left Arrow rotate left the car base
        case 37: {
            palanque.data.rotateCW = true;
            break;
        }
        // Right Arrow rotate right the car base
        case 39: {
            palanque.data.rotateCCW = true;
            break;
        }
    }
}

function onKeyUp(e) {
    'use strict';
    switch (e.keyCode) {
        case 37: {
            palanque.data.rotateCW = false;
            break;
        }
        case 39: {
            palanque.data.rotateCCW = false;
            break;
        }
    }
}

function render() {
    'use strict';
    renderer.render(scene, currentCamera);
}

function rotatePalanque(step) {
    'use strict';

    if (palanque.data.rotateCW && !palanque.data.rotateCCW) {
        palanque.rotateY(-step);
    } else if (!palanque.data.rotateCW && palanque.data.rotateCCW) {
        palanque.rotateY(step);
    }
}

function checkIluminationCalcs() {
    'use strict';
    if (toggleIluminationCalcs) {
        if (calcsEnabled) {
            meshMaterials.forEach(materialsArray => {
                materialsArray[3].material = materialsArray[0];
            });
        } else {
            meshMaterials.forEach(materialsArray => {
                materialsArray[3].material = materialsArray[2];
            });
        }
        calcsEnabled = !calcsEnabled;
        toggleIluminationCalcs = false;
    }
}

function checkChangeShading() {
    'use strict';
    if (changeShading) {
        if (calcsEnabled) {
            if (phongEnabled) {
                meshMaterials.forEach(materialsArray => {
                    materialsArray[3].material = materialsArray[1];
                });
            } else {
                meshMaterials.forEach(materialsArray => {
                    materialsArray[3].material = materialsArray[2];
                }); 
            }
            phongEnabled = !phongEnabled;
        }
        changeShading = false;
    }
}

function init() {
    'use strict';
    renderer = new THREE.WebGLRenderer({
        antialias: true
    });
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    createScene();
    createCameras();

    clock = new THREE.Clock();

    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);
    window.addEventListener("resize", onResize);
}

function animate() {
    'use strict';
    var step = clock.getDelta();
    if (changeToCamera !== currentCamera) {
        currentCamera = changeToCamera
        onResize();
    }

    rotatePalanque(step);
    checkChangeShading();
    checkIluminationCalcs();
    render();
    requestAnimationFrame(animate);
}
