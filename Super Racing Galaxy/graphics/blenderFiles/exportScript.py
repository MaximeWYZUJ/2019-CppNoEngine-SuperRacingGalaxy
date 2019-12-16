import bpy

context = bpy.context
scene = context.scene
objects = scene.objects

planete = "planete2"

f = open("E:/Cours/Projet-Automne/Super Racing Galaxy/graphics/blenderFiles/" + planete + ".txt", "w+")

f.write("Planet *" + planete + " = new Planet(Transform<>({ 0.0f, 0.0f, 0.0f }, { 200.0f, 200.0f, 200.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), -9.81f * 10, true, \"graphics/meshs/" + planete + ".obj\", L\"graphics/textureDDS/" + planete + "/" + planete + "Texture2.dds\");\n\n{\n")
        
cptTeleport = 0
for obj in objects:
    if(obj.parent):
        objName, other = obj.name.split('.')
        if(objName != "teleporteur"):
            f.write("\t" + planete + "->addElement(new Scenery(Transform<>::BlenderToCookie({" + str(obj.location[0]) + "f, " + str(obj.location[1]) + "f, " + str(obj.location[2]) + "f }, { " + str(obj.scale[0]) + "f, " + str(obj.scale[1]) + "f, " + str(obj.scale[2]) + "f }, { " + str(obj.rotation_quaternion[0]) + ", " + str(obj.rotation_quaternion[1]) + ", " + str(obj.rotation_quaternion[2]) + ", " + str(obj.rotation_quaternion[3]) + " }), \"graphics/meshs/" + objName + ".obj\", L\"graphics/textureDDS/" + objName + "Texture.dds\"));\n")
        else:
            cptTeleport += 1
            f.write("\n\tTeleport* teleporteur" + str(cptTeleport) + " = new Teleport(Transform<>::BlenderToCookie({" + str(obj.location[0]) + "f, " + str(obj.location[1]) + "f, " + str(obj.location[2]) + "f }, { " + str(obj.scale[0]) + "f, " + str(obj.scale[1]) + "f, " + str(obj.scale[2]) + "f }, { " + str(obj.rotation_quaternion[0]) + ", " + str(obj.rotation_quaternion[1]) + ", " + str(obj.rotation_quaternion[2]) + ", " + str(obj.rotation_quaternion[3]) + " }), \"graphics/meshs/teleporteur.obj\", L\"graphics/textureDDS/teleporteurTexture.dds\");\n")
            f.write("\t" + planete + "->addTeleport(teleporteur" + str(cptTeleport) + ");\n\n")
            f.write("\tteleporteur" + str(cptTeleport) + "->linkedTeleport = nullptr;\n\n")
            
f.write("}")
f.close()