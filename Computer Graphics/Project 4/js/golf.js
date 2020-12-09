var normalScene, renderer, pausedScene;

var perspectiveCamera, ortoCamera;

var controls;

var pointLight, ambientLight, directionalLight;
var golfBall, bandeira, bandeiraFlag;

const loader = new THREE.TextureLoader();

var pause = false, changeWireframe = false, changeIlumination = false, calcsEnabled = true;
var resetFlag = false;

var bandeiraY = 0;

var bolaX = 50, bolaY = 20, bolaZ = -50;
var posCameraX = 250, posCameraY = 250, posCameraZ = 250;

var meshMaterials = [];

var clock;

const FACTOR = 3.5;

function createGolfField() {
    'use strict';
    var obj = new THREE.Object3D();
    var relva = new THREE.BoxGeometry(4000, 2, 4000);

    var texture = loader.load("./images/grassTexture.png");
    texture.wrapS = texture.wrapT = THREE.TextureRepeat;
    texture.repeat.set(4,4);

    var textureBump = loader.load("./images/grassBumpMap.jpg");
    textureBump.wrapS = textureBump.wrapT = THREE.TextureRepeat;

    var phongMat = new THREE.MeshPhongMaterial({
        map: texture,
        bumpMap: textureBump,
        wireframe: false
    });

    var basicMat = new THREE.MeshBasicMaterial({
        map: texture,
        wireframe: false
    });
    
    var mesh = new THREE.Mesh(relva, phongMat);
    meshMaterials.push([basicMat, phongMat, mesh]);

    mesh.position.set(0, 0, 0);
    obj.add(mesh);
    normalScene.add(obj);
    return obj;
}

function createGolfBall() {
    'use strict';
    var obj = new THREE.Object3D();
    obj.userData = { jumping: false, step: 0 };
    var bola = new THREE.SphereGeometry(20, 100, 100);

    var texture = loader.load("./images/ballNormalMap.jpg");
    texture.wrapS = texture.wrapT = THREE.RepeatWrapping;

    var phongMat = new THREE.MeshPhongMaterial({
        normalMap: texture,
        wireframe: false
    });
    
    var basicMat = new THREE.MeshBasicMaterial({
        wireframe: false
    });
    
    var mesh = new THREE.Mesh(bola, phongMat);

    meshMaterials.push([basicMat, phongMat, mesh]);
    
    obj.add(mesh);
    normalScene.add(obj);
    obj.translateX(bolaX);
    obj.translateY(bolaY);
    obj.translateZ(bolaZ);
    return obj;
}

function createBandeira() {
    'use strict'
    var obj = new THREE.Object3D();

    var geometry = new THREE.CylinderGeometry(5, 5, 200, 32);
    var phongMaterial = new THREE.MeshPhongMaterial({ color: 0xffff00, wireframe: false });
    var basicMaterial = new THREE.MeshBasicMaterial({ color: 0xffff00, wireframe: false });
    var poste = new THREE.Mesh(geometry, phongMaterial);

    meshMaterials.push([basicMaterial, phongMaterial, poste]);
    
    poste.translateY(100);
    obj.add(poste);

    var bandeiraGeo = new THREE.BoxGeometry(8, 40, 60);
    var bandeiraPhongMat = new THREE.MeshPhongMaterial({ color: 0xff0000, wireframe: false });
    var bandeiraBasicMat = new THREE.MeshBasicMaterial({ color: 0xff0000, wireframe: false });
    bandeiraFlag = new THREE.Mesh(bandeiraGeo, bandeiraPhongMat);

    meshMaterials.push([bandeiraBasicMat, bandeiraPhongMat, bandeiraFlag]);

    bandeiraFlag.translateY(175);
    bandeiraFlag.translateZ(30);
    obj.add(bandeiraFlag);

    obj.rotateY(Math.PI/2);

    normalScene.add(obj);
    return obj;
}

function createGamePause() {
    'use strict';
    var obj = new THREE.Object3D();
    var pause = new THREE.BoxGeometry(2, 336, 600);
    var texture = loader.load("./images/paused.jpeg");
    var mat = new THREE.MeshBasicMaterial({ map: texture });
    var mesh = new THREE.Mesh(pause, mat);
    
    obj.translateX(10);
    obj.add(mesh);
    pausedScene.add(obj);
    return obj;
}

function createDirectionalLight() {
    'use strict';
    directionalLight = new THREE.DirectionalLight(0xffffff, 0.5);
    directionalLight.translateX(100);
    normalScene.add(directionalLight);
}

function createPointLight() {
    'use strict';
    var light = new THREE.PointLight(0xffffff, .6, 500);
    light.position.set(0, 150, 0);
    normalScene.add(light);
    return light;
}

function createAmbientLight() {
    'use strict';
    var light = new THREE.AmbientLight(0x505050);
    normalScene.add(light);
    return light;
}

function createGolfScene() {
    'use strict';

    var grass = createGolfField();
    createDirectionalLight();
    directionalLight.target = grass;

    golfBall = createGolfBall();
    bandeira = createBandeira();

    pointLight = createPointLight();
    ambientLight = createAmbientLight();
}

function createScene() {
    'use strict';
    normalScene = new THREE.Scene();
    pausedScene = new THREE.Scene();
    createGolfScene();
    createGamePause();
}

function createCameras() {
    'use strict';
    perspectiveCamera = createPerspectiveCamera(250, 250, 250);
    ortoCamera = createOrthographicCamera(-100, 0, 0);
}

function createOrthographicCamera(x, y, z) {
    'use strict';
    var camera = new THREE.OrthographicCamera(window.innerWidth / -FACTOR, window.innerWidth / FACTOR, window.innerHeight / FACTOR, window.innerHeight / -FACTOR, 1, 1000 );
    camera.position.x = x;
    camera.position.y = y;
    camera.position.z = z;
    camera.lookAt(pausedScene.position);
    return camera;

}
function createPerspectiveCamera(x, y, z) {
    'use strict';
    var camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 1, 20000);
    camera.position.x = x;
    camera.position.y = y;
    camera.position.z = z;
    camera.lookAt(golfBall.position);
    return camera;
}

function onResize() {
    'use strict';
    renderer.setSize(window.innerWidth, window.innerHeight);
    
    if (window.innerHeight > 0 && window.innerWidth > 0) {
        currentCamera.aspect = window.innerWidth / window.innerHeight;

        if (pause) {
            currentCamera.left = window.innerWidth / -FACTOR;
            currentCamera.right = window.innerWidth / FACTOR;
            currentCamera.top = window.innerHeight / FACTOR;
            currentCamera.bottom = window.innerHeight / -FACTOR;
        }
        currentCamera.updateProjectionMatrix();
    }
}


function onKeyDown(e) {
    'use strict';
    switch (e.keyCode) {
        // D, d
        case 68:
        case 100: {
            directionalLight.visible = !directionalLight.visible;
            break;
        }
        // P, p
        case 80:
        case 112: {
            pointLight.visible = !pointLight.visible;
            break;
        }
        // W, w
        case 87:
        case 119: {
            changeWireframe = true;
            break;
        }
        // I, i
        case 73:
        case 105: {
            changeIlumination = true;
            break;
        }
        // S, s
        case 83:
        case 115: {
            pause = !pause;
            break;
        }
        // B, b
        case 66:
        case 98: {
            golfBall.userData.jumping = !golfBall.userData.jumping;
            break;
        }
        //R, r
        case 82:
        case 114: {
            resetFlag = true;
            break;
        }
    }
}

function render() {
    'use strict';
    renderer.autoClear = false; 
    renderer.clear(); 
    renderer.render(normalScene, perspectiveCamera);
    if (pause) {
        renderer.clearDepth(); 
        renderer.render(pausedScene, ortoCamera);
    }
}

function reset() {
    'use strict';
    if (resetFlag && pause) {

        golfBall.position.x = bolaX;
        golfBall.position.y = bolaY;
        golfBall.position.z = bolaZ;

        bandeira.rotation.y = Math.PI/2;

        meshMaterials.forEach(materialsArray => materialsArray[0].wireframe = materialsArray[1].wireframe = false);
        meshMaterials.forEach(materialsArray => {
            materialsArray[2].material = materialsArray[1];
        });

        perspectiveCamera.position.x = posCameraX;
        perspectiveCamera.position.y = posCameraY;
        perspectiveCamera.position.z = posCameraZ;
        changeIlumination = changeWireframe = golfBall.userData.jumping = pause = false;
        pointLight.visible = directionalLight.visible = true;
    }
    resetFlag = false;
}

function checkIluminationCalcs() {
    'use strict';
    if (changeIlumination) {
        if (calcsEnabled) {
            meshMaterials.forEach(materialsArray => {
                materialsArray[2].material = materialsArray[0];
            });
        } else {
            meshMaterials.forEach(materialsArray => {
                materialsArray[2].material = materialsArray[1];
            });
        }
        calcsEnabled = !calcsEnabled;
        changeIlumination = false;
    }
}

function createPathStrings() {
    'use strict';
    const basePath = "./images/";
    const fileType = ".png";
    const sides = ["skyFt", "skyBk", "skyUp", "skyDn", "skyLf", "skyRt"];
    const pathStings = sides.map(side => {
      return basePath + side + fileType;
    });
    return pathStings;
}

function createMaterialArray() {
    'use strict';
    const skyboxImagepaths = createPathStrings();
    const materialArray = skyboxImagepaths.map(image => {
      let texture = new THREE.TextureLoader().load(image);
      return new THREE.MeshBasicMaterial({ map: texture, side: THREE.BackSide }); 
    });
    return materialArray;
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

    controls = new THREE.OrbitControls(perspectiveCamera, renderer.domElement);
    controls.update();

    var materialSkybox = createMaterialArray();
    var skyBoxGeo = new THREE.BoxBufferGeometry(10000, 10000, 10000);
    var skybox = new THREE.Mesh(skyBoxGeo, materialSkybox);
    normalScene.add(skybox);

    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("resize", onResize);
}

function animateBall() {
    'use strict';
    if (golfBall.userData.jumping) {
        golfBall.userData.step += 0.04;
        golfBall.position.y = 20 + Math.abs(100 * (Math.sin(golfBall.userData.step)));
        golfBall.position.z = -50 * (Math.cos(golfBall.userData.step));
    }
}

function animateFlag(step) {
    'use strict';
    bandeira.rotateY(step);
}

function stopAndContinue(step) {
    'use strict';
    if (!pause) {
        animateBall();
        animateFlag(step);
    }
}

function animate() {
    'use strict';
    var step = clock.getDelta();

    if (pause) {
        controls.enabled = false;
    } else {
        controls.enabled = true;
        controls.update();
        checkIluminationCalcs();
    
        if (changeWireframe) {
            meshMaterials.forEach(materialsArray => materialsArray[0].wireframe = materialsArray[1].wireframe = !materialsArray[0].wireframe);
            changeWireframe = false;
        }
        stopAndContinue(step);
    }
    reset();
    render();
    requestAnimationFrame(animate);
}
