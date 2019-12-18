import bpy

context = bpy.context
scene = context.scene
objects = scene.objects


for obj in objects:
    if(not obj.parent):
        objPlanete = obj

planete = objPlanete.name
f = open("E:/Cours/Projet-Automne/Super Racing Galaxy/graphics/blenderFiles/" + planete + ".txt", "w+")
f.write("Planet *" + planete + " = new Planet(Transform<>({ -500.0f, 250.0f, 300.0f }, { 400.0f, 400.0f, 400.0f }, { 0.0f, 0.0f, 0.0f, 0.1f }), -9.81f * 10, true, \"graphics/meshs/" + planete + ".obj\", L\"graphics/textureDDS/" + planete + "/" + planete + "Texture2.dds\");\n\n{\n")
              
cptTeleport = 0
cptAtterrissage = 0
for obj in objects:
    if(obj.parent):
        obj.rotation_mode = 'QUATERNION'
        objName, other = obj.name.split('.')
        if(objName == "pisteAtterrissage"):
            cptAtterrissage += 1
            f.write("\n\tLanding* piste" + str(cptAtterrissage) + " = new Landing(Transform<>::BlenderToCookie({" + str(obj.location[0]) + "f, " + str(obj.location[1]) + "f, " + str(obj.location[2]) + "f }, { " + str(obj.scale[0]) + "f, " + str(obj.scale[1]) + "f, " + str(obj.scale[2]) + "f }, { " + str(obj.rotation_quaternion[0]) + ", " + str(obj.rotation_quaternion[1]) + ", " + str(obj.rotation_quaternion[2]) + ", " + str(obj.rotation_quaternion[3]) + " }), \"graphics/meshs/pisteAtterrissage.obj\", L\"graphics/textureDDS/pisteAtterrissageTexture.dds\");\n")
            f.write("\t" + planete + "->addElement(piste" + str(cptAtterrissage) + ");\n\n")
        elif(objName == "trophee"):
            f.write("\tGoal *goal = new Goal(Transform<>({ " + str(obj.location[0]) + "f, " + str(obj.location[1]) + "f, " + str(obj.location[2]) + "f }, { " + str(obj.scale[0]) + "f, " + str(obj.scale[1]) + "f, " + str(obj.scale[2]) + "f }, { " + str(obj.rotation_quaternion[0]) + ", " + str(obj.rotation_quaternion[1]) + ", " + str(obj.rotation_quaternion[2]) + ", " + str(obj.rotation_quaternion[3]) + " }), \"graphics/meshs/trophee.obj\", L\"graphics/textureDDS/tropheeTexture.dds\", \"graphics/meshs/hitBox/hitBoxTrophee.obj\");\n")
            f.write("\t" + planete + "->addGoal(goal);\n")
            f.write("\t" + "scenario.goal = goal;\n")
        elif(objName == "teleporteur"):
            cptTeleport += 1
            f.write("\t" + planete + "->addElement(new Scenery(Transform<>::BlenderToCookie({" + str(obj.location[0]) + "f, " + str(obj.location[1]) + "f, " + str(obj.location[2]) + "f }, { " + str(obj.scale[0]) + "f, " + str(obj.scale[1]) + "f, " + str(obj.scale[2]) + "f }, { " + str(obj.rotation_quaternion[0]) + ", " + str(obj.rotation_quaternion[1]) + ", " + str(obj.rotation_quaternion[2]) + ", " + str(obj.rotation_quaternion[3]) + " }), \"graphics/meshs/teleporteur.obj\", L\"graphics/textureDDS/teleporteurTexture.dds\", \"graphics/meshs/teleporteur.obj\"));\n")
            f.write("\n\tTeleport* teleporteur" + str(cptTeleport) + " = new Teleport(Transform<>::BlenderToCookie({" + str(obj.location[0]) + "f, " + str(obj.location[1]) + "f, " + str(obj.location[2]) + "f }, { " + str(obj.scale[0]) + "f, " + str(obj.scale[1]) + "f, " + str(obj.scale[2]) + "f }, { " + str(obj.rotation_quaternion[0]) + ", " + str(obj.rotation_quaternion[1]) + ", " + str(obj.rotation_quaternion[2]) + ", " + str(obj.rotation_quaternion[3]) + " }), \"graphics/meshs/teleporteur.obj\", L\"graphics/textureDDS/teleporteurTexture.dds\", \"graphics/meshs/hitBox/hitBoxTriggerTeleporteur.obj\");\n")
            f.write("\t" + planete + "->addTeleport(teleporteur" + str(cptTeleport) + ");\n\n")
            f.write("\tteleporteur" + str(cptTeleport) + "->linkedTeleport = nullptr;\n\n")
        else:
            f.write("\t" + planete + "->addElement(new Scenery(Transform<>::BlenderToCookie({" + str(obj.location[0]) + "f, " + str(obj.location[1]) + "f, " + str(obj.location[2]) + "f }, { " + str(obj.scale[0]) + "f, " + str(obj.scale[1]) + "f, " + str(obj.scale[2]) + "f }, { " + str(obj.rotation_quaternion[0]) + ", " + str(obj.rotation_quaternion[1]) + ", " + str(obj.rotation_quaternion[2]) + ", " + str(obj.rotation_quaternion[3]) + " }), \"graphics/meshs/" + objName + ".obj\", L\"graphics/textureDDS/" + objName + "Texture.dds\", \"graphics/meshs/hitBox/hitBox" + objName + ".obj\"));\n")
        
f.write("}")
f.close()