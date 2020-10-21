var scene, renderer;

// The different levels on the mobile
var v1, v2, v3;

var frontCamera, topCamera, sideCamera, currentCamera;

// Flag to changeCameraEvent
var changeToCamera;

// Flag to change the wireframe
var changeWireframe = false;

// Array to keep the materials in order to change their wireframe
var materials = [];

var goLeft = goRight = goFront = goBack = false;

var clock;

// Factor to set the camera view
const FACTOR = 3.5;
// To speed the mobile movement
const SPEED_UP = 50;

function createCylinder(obj, x, y, z, r1, r2, h, rotX, rotY, rotZ, scaleX, scaleY, scaleZ, color) {
    'use strict';

    var material = new THREE.MeshBasicMaterial({ color: color, wireframe: true });
    var geometry = new THREE.CylinderGeometry(r1, r2, h, 32);
    geometry.rotateX(rotX).rotateY(rotY).rotateZ(rotZ);
    geometry.scale(scaleX, scaleY, scaleZ);

    var mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
    materials.push(material);    
}

function createCube(obj, x, y, z, l, c, h, rotX, rotY, rotZ, color) {
    'use strict';

    var material = new THREE.MeshBasicMaterial({ color: color, wireframe: true });
    var geometry = new THREE.BoxGeometry(l, c, h);
    geometry.rotateX(rotX).rotateY(rotY).rotateZ(rotZ);

    var mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
    materials.push(material);    
}

function createScene() {
    'use strict';

    scene = new THREE.Scene();

    v1 = createV1().translateY(130);
    v2 = createV2().translateX(60).translateY(-45);
    v3 = createV3().translateX(50).translateY(-95);

    // v2 is v1 child and v3 is v2 child
    v1.add(v2);
    v2.add(v3);
}

function createV1() {
    'use strict';
    v1 = new THREE.Object3D();
    v1.userData = {rotateCW: false, rotateCCW: false};
    
    // rods v1
    createCylinder(v1, 0, 0, 0, 0.2, 0.2, 50, 0, 0, 0, 1, 1, 1, 0xffb6c1);
    createCylinder(v1, 25, -25, 0, 0.2, 0.2, 110, 0, 0, Math.PI / 2, 1, 1, 1, 0xffb6c1);
    createCylinder(v1, -20, -40, 0, 0.2, 0.2, 30, 0, 0, 0, 1, 1, 1, 0xffb6c1);
    createCylinder(v1, -20, -55, 0, 0.2, 0.2, 50, Math.PI / 4, 0, Math.PI / 2, 1, 1, 1, 0xffb6c1);

    // toys v1
    createCube(v1, -35, -25, 0, 10, 20, 20, 0, 0, 0, 0xff0000);
    createCylinder(v1, 90, -25, 0, 10, 10, 30, Math.PI / 4, 0, 0, 1, 1, 1, 0x55ff00);
    createCylinder(v1, 60, -5, 0, 10, 10, 20, 0, 0, 0, 1, 1, 0.5, 0x55ff00);
    createCube(v1, -15.5 + 25 * Math.cos(Math.PI / 4), -55, -3.5 - 25 * Math.sin(Math.PI / 4), 10, 20, 20, 0, Math.PI / 4, 0, 0xff0000);
    createCylinder(v1, -27 - 25 * Math.cos(Math.PI / 4), -55, 7 + 25 * Math.sin(Math.PI / 4), 10, 10, 15, 0, 0, 0, 1, 1, 1, 0x0000ff);
    scene.add(v1);
    return v1;
}

function createV2() {
    'use strict';

    v2 = new THREE.Object3D();
    v2.userData = {rotateCW: false, rotateCCW: false};
    // rods v2
    createCylinder(v2, 0, 0, 0, 0.2, 0.2, 60, 0, 0, 0, 1, 1, 1, 0xffb6c1);
    createCylinder(v2, 20, -30, 0, 0.2, 0.2, 110, 0, 0, Math.PI / 2, 1, 1 ,1, 0xffb6c1); 
    createCylinder(v2, 60, -45, 0, 0.2, 0.2, 30, 0, 0, 0, 1, 1, 1, 0xffb6c1);
    createCylinder(v2, 60, -60, 0, 0.2, 0.2, 50, 0, 0, Math.PI / 2, 1, 1, 1, 0xffb6c1);
    
    //toys v2
    createCube(v2, 90, -30, 0, 30, 5, 30, 0, 0, 0, 0xff850b);
    createCylinder(v2, -40, -30, 0, 7, 7, 10, 0, 0, Math.PI / 2, 1, 1, 1, 0xffff00);
    createCube(v2, 92.5, -60, 0, 15, 15, 15, Math.PI/3, 0, 0, 0xff850b);
    createCylinder(v2, 25, -60, 0, 10, 10, 20, 0, 0, Math.PI / 2, 1, 1, 0.5, 0xff850b);
    
    scene.add(v2);
    return v2;
}

function createV3() {
    'use strict';

    v3 = new THREE.Object3D();
    v3.userData = {rotateCW: false, rotateCCW: false};
    // rods v3
    createCylinder(v3, 0, 0, 0, 0.2, 0.2, 70, 0, 0, 0, 1, 1, 1, 0xffb6c1);
    createCylinder(v3, 0, -35, 0, 0.2, 0.2, 40, 0, 0, Math.PI / 2, 1, 1, 1, 0xffb6c1);
    
    // toys v3
    createCylinder(v3, 22.5, -35, 0, 5, 5, 40, 0, 0, 0, 1, 1, 1, 0xffff00);
    createCube(v3, -22.5, -35, 0, 5, 20, 5, 0, 0, 0, 0xffff00);

    scene.add(v3);
    return v3;
}

function createCameras() {
    'use strict';

    currentCamera = changeToCamera = frontCamera = createCamera(frontCamera, 0, 0, 300);
    topCamera = createCamera(topCamera, 0, 300, 0);
    sideCamera = createCamera(sideCamera, 300, 0, 0);
}

function createCamera(cameraToCreate, x, y, z) {
    'use strict';
    cameraToCreate = new THREE.OrthographicCamera(window.innerWidth / -FACTOR, window.innerWidth / FACTOR, window.innerHeight / FACTOR, window.innerHeight / -FACTOR, 1, 1000 );
    cameraToCreate.position.x = x;
    cameraToCreate.position.y = y;
    cameraToCreate.position.z = z;
    cameraToCreate.lookAt(scene.position);
    return cameraToCreate;
}

function onResize() {
    'use strict';
    renderer.setSize(window.innerWidth, window.innerHeight);
    
    if (window.innerHeight > 0 && window.innerWidth > 0) {
        currentCamera.left = window.innerWidth / -FACTOR;
        currentCamera.right = window.innerWidth / FACTOR;
        currentCamera.top = window.innerHeight / FACTOR;
        currentCamera.bottom = window.innerHeight / -FACTOR;
        currentCamera.updateProjectionMatrix();
    }
}

function onKeyDown(e) {
    'use strict';

    switch (e.keyCode) {
        // Q, q, v1 start rotating clockwise
        case 81:
        case 113: {
            v1.userData.rotateCW = true;
            break;
        }
        // E, e, v1 start rotating counterclockwise
        case 69:
        case 101: {
            v1.userData.rotateCCW = true;
            break;
        }
        // A, a, v2 start rotating clockwise
        case 65:
        case 97: {
            v2.userData.rotateCW = true;
            break;
        }
        // D, d, v2 start rotating counterclockwise
        case 68:
        case 100: {
            v2.userData.rotateCCW = true;
            break;
        }
        // Z, z, v3 start rotating clockwise
        case 90:
        case 122: {
            v3.userData.rotateCW = true;
            break;
        }
        // C, c, v3 start rotating counterclockwise
        case 67:
        case 99: {
            v3.userData.rotateCCW = true;
            break;
        }
        // 1, Select Front Camera
        case 49: {
            changeToCamera = frontCamera;
            break;
        }
        // 2, Select Top Camera
        case 50: {
            changeToCamera = topCamera;
            break;
        }
        // 3, Select Side Camera
        case 51: {
            changeToCamera = sideCamera;
            break;
        }
        // 4, Enable Wireframe
        case 52: {
            changeWireframe = true;
            break;
        }
        // Left arrow, start translate left
        case 37: {
            goLeft = true;
            break;
        }
        // Up arrow, start translate front
        case 38: {
            goBack = true;
            break;
        }
        // Right arrow, start translate right
        case 39: {
            goRight = true;
            break;
        }
        // Down arrow, start translate down
        case 40: {
            goFront = true;
            break;
        }
    }
}

function onkeyUp(e) {
    'use strict';

    switch (e.keyCode) {
        // Q, q, v1 stop rotating clockwise 
        case 81:
        case 113: {
            v1.userData.rotateCW = false;
        }
        // E, e, stop rotating counterclockwise
        case 69:
        case 101: {
            v1.userData.rotateCCW = false;
            break;
        }
        // A, a, v2 stop rotating clockwise
        case 65:
        case 97: {
            v2.userData.rotateCW = false;
        }
        // D, d, v2 stop rotating counterclockwise
        case 68:
        case 100: {
            v2.userData.rotateCCW = false;
            break;
        }
        // Z, z, v3 stop rotating clockwise
        case 90:
        case 122: {
            v3.userData.rotateCW = false;
            break;
        }
        // C, c, v3 stop rotating counterclockwise
        case 67:
        case 99: {
            v3.userData.rotateCCW = false;
            break;
        }
        // Left arrow, stop translate left
        case 37: {
            goLeft = false;
            break;
        }
        // Up arrow, stop translate front
        case 38: {
            goBack = false;
            break;
        }
        // Right arrow, stop translate right
        case 39: {
            goRight = false;
            break;
        }
        // Down arrow, stop translate down
        case 40: {
            goFront = false;
            break;
        }
    }
}

function render() {
    'use strict';
    renderer.render(scene, currentCamera);
}

function checkTranslations(step) {
    'use strict';

    if (goLeft) {
        v1.position.x -= step * SPEED_UP;
    }
    if (goRight) {
        v1.position.x += step * SPEED_UP;
    }
    if (goBack) {
        v1.position.z -= step * SPEED_UP;
    }
    if (goFront) {
        v1.position.z += step * SPEED_UP;
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
    window.addEventListener("keyup", onkeyUp);
    window.addEventListener("resize", onResize);
}

function rotateMobile(object, step) {
    'use strict';

    if (object.userData.rotateCW && !object.userData.rotateCCW) {
        object.rotateY(step);
    } else if (!object.userData.rotateCW && object.userData.rotateCCW) {
        object.rotateY(-step);
    }
}

function animate() {
    'use strict';

    var step = clock.getDelta();
    if (changeToCamera !== currentCamera) {
        currentCamera = changeToCamera
        onResize();
    }   

    if (changeWireframe) {
        materials.forEach(material => material.wireframe = !material.wireframe);
        changeWireframe = false;
    }

    if (v1.userData.rotateCW || v1.userData.rotateCCW) {
        rotateMobile(v1, step);
    }

    if (v2.userData.rotateCW || v2.userData.rotateCCW) {
        rotateMobile(v2, step);
    }

    if (v3.userData.rotateCW || v3.userData.rotateCCW) {
        rotateMobile(v3, step);
    }

    checkTranslations(step) ;
    render();
    requestAnimationFrame(animate);
}
