#include "pch.h"
#include "DemoScenario.h"
#include "Planet.h"
#include "Vehicle.h"
#include "Teleport.h"
#include "Scenery.h"
#include "Vector3.h"
#include "Skybox.h"

using namespace Cookie;

Scenario ScenarioCreator::CreateDemoScenario()
{
	Scenario scenario;

	/*Planet *planete1 = new Planet(Transform<>({ 0.0f, 0.0f, 0.0f }, { 500.0f, 500.0f, 500.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
		-9.81f, true, "graphics\\meshs\\planete1test2.obj", L"graphics\\textureDDS\\planete1\\planete1Alpha.dds");

	{
		planete1->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.0f, 0.0f, 1.0f }, { 1.0f, 0.1f, 1.0f }, { 0.5f, -0.5f, -0.5f, 0.5f }),
			"graphics\\meshs\\champi2.obj", L"graphics\\textureDDS\\champi2Texture.dds"));
	}*/

	scenario.skybox = new Skybox(Transform<>({ 0.0f, 0.0f, 0.00001f }, { 10000.0f, 10000.0f, 10000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), "graphics/meshs/skybox.obj", L"graphics/textureDDS/skyboxTexture.dds");

	Planet *planete2 = new Planet(Transform<>({ 0.0f, 0.0f, 0.0f }, { 200.0f, 200.0f, 200.0f }, { 0.0f, 0.0f, 0.707f, 0.707f }), -9.81f * 10, true, "graphics/meshs/planete2.obj", L"graphics/textureDDS/planete2/planete2Texture2.dds");

	{
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.44991758465766907f, 0.0174355860799551f, -0.20012310147285461f }, { 0.1499950885772705f, 0.14999498426914215f, 0.2999899089336395f }, { 0.6058107614517212, -0.7750918865203857, 0.05823089927434921, 0.16980868577957153 }), "graphics/meshs/bonhommeNeige.obj", L"graphics/textureDDS/bonhommeNeigeTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.4768911600112915f, -0.3066578507423401f, -0.021480485796928406f }, { 0.09270559251308441f, 0.09270557761192322f, 0.18541111052036285f }, { 0.6938583850860596, 0.2537615895271301, -0.6709803342819214, -0.06285690516233444 }), "graphics/meshs/bonhommeNeige.obj", L"graphics/textureDDS/bonhommeNeigeTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.2274918407201767f, 0.03310556709766388f, 0.4983408451080322f }, { 0.020684342831373215f, 0.020684324204921722f, 0.04136865213513374f }, { 0.7710033059120178, 0.036688558757305145, -0.039404381066560745, -0.6345511078834534 }), "graphics/meshs/bonhommeNeige.obj", L"graphics/textureDDS/bonhommeNeigeTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.21352627873420715f, 0.062308311462402344f, 0.5027865171432495f }, { 0.02501821331679821f, 0.025018204003572464f, 0.05003640055656433f }, { 0.9906410574913025, -0.008501547388732433, -0.024720488116145134, -0.13396631181240082 }), "graphics/meshs/bonhommeNeige.obj", L"graphics/textureDDS/bonhommeNeigeTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.20619171857833862f, 0.09459058195352554f, 0.49836140871047974f }, { 0.017991431057453156f, 0.01799141988158226f, 0.035982850939035416f }, { 0.8728851079940796, 0.08543853461742401, -0.06011045724153519, 0.47661152482032776 }), "graphics/meshs/bonhommeNeige.obj", L"graphics/textureDDS/bonhommeNeigeTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.4586777091026306f, -1.5997327864170074e-05f, 0.3009292185306549f }, { 0.1999933421611786f, 0.19999323785305023f, 0.09999663382768631f }, { -9.637899722747534e-08, 0.5172414779663086, 1.3029636036776537e-08, 0.8558394908905029 }), "graphics/meshs/igloo.obj", L"graphics/textureDDS/iglooTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.10713794082403183f, -0.3241073787212372f, -0.36107343435287476f }, { 0.001999932574108243f, 0.01999932900071144f, 0.019999327138066292f }, { -0.19898957014083862, -0.8851639628410339, 0.37427735328674316, -0.19184470176696777 }), "graphics/meshs/panneau.obj", L"graphics/textureDDS/panneauTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.11313480138778687f, -0.3286808431148529f, -0.373332142829895f }, { 0.0019999328069388866f, 0.019999338313937187f, 0.019999325275421143f }, { 0.20061549544334412, 0.4137773811817169, 0.8674023747444153, -0.19014424085617065 }), "graphics/meshs/panneau.obj", L"graphics/textureDDS/panneauTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.08020666241645813f, -0.1643260270357132f, 0.5058423280715942f }, { 0.001999933272600174f, 0.01999933272600174f, 0.019999327138066292f }, { 0.45550864934921265, 0.0, 0.0, -0.8902313709259033 }), "graphics/meshs/panneau.obj", L"graphics/textureDDS/panneauTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.08499567955732346f, -0.1609691083431244f, 0.4927034080028534f }, { 0.0019999323412775993f, 0.019999323412775993f, 0.019999321550130844f }, { 0.8959521651268005, 0.004284471273422241, 0.001703950809314847, 0.444126695394516 }), "graphics/meshs/panneau.obj", L"graphics/textureDDS/panneauTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.3725969195365906f, -0.30911576747894287f, 0.28954923152923584f }, { 0.04999830573797226f, 0.0999966487288475f, 0.0999966710805893f }, { -0.2713140547275543, -0.45059412717819214, 0.18905620276927948, -0.8292233347892761 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.2815207540988922f, 0.222176194190979f, 0.5019234418869019f }, { 0.0831971988081932f, 0.16599443554878235f, 0.16599442064762115f }, { -0.4409595727920532, 0.3152017891407013, -0.0561874620616436, -0.8384780287742615 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.17776404321193695f, -0.3077206611633301f, 0.5014307498931885f }, { 0.08319717645645142f, 0.16599445044994354f, 0.16599449515342712f }, { 0.026106618344783783, 0.1415065973997116, 0.27203232049942017, -0.9514687657356262 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.5175397396087646f, 0.20321093499660492f, 0.08141198754310608f }, { 0.08319719135761261f, 0.16599445044994354f, 0.16599445044994354f }, { -0.32138070464134216, 0.6952767968177795, 0.11700895428657532, -0.6321499347686768 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.5683639645576477f, -0.046750642359256744f, 0.2320539951324463f }, { 0.13099558651447296f, 0.26299118995666504f, 0.26299116015434265f }, { 0.5449667572975159, -0.4018142521381378, -0.4367212653160095, 0.5923099517822266 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.16185565292835236f, -0.3963949978351593f, 0.39841240644454956f }, { 0.05288288742303848f, 0.10576575249433517f, 0.10576573014259338f }, { -0.018986660987138748, -0.15560877323150635, -0.402644544839859, 0.9018329977989197 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.3116014003753662f, 0.4503456652164459f, 0.10893809050321579f }, { 0.08318585157394409f, 0.16637162864208221f, 0.1663716435432434f }, { 0.7882840037345886, -0.5171591639518738, 0.3234269320964813, -0.08093193173408508 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.02855491451919079f, 0.13622678816318512f, -0.5517236590385437f }, { 0.08318577706813812f, 0.16637159883975983f, 0.1663716584444046f }, { -0.0130043625831604, -0.11329146474599838, -0.9836422801017761, -0.1394423246383667 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.3148795962333679f, -0.16816963255405426f, -0.4042126536369324f }, { 0.055672746151685715f, 0.11134547740221024f, 0.11134548485279083f }, { -0.3282487988471985, -0.283050537109375, -0.8997206687927246, 0.05135872960090637 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.31395313143730164f, 0.4570569396018982f, -0.1579245626926422f }, { 0.08318577706813812f, 0.16637158393859863f, 0.1663716584444046f }, { 0.5232884883880615, -0.3152288794517517, -0.7363483309745789, -0.29084575176239014 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.1458444446325302f, 0.19209782779216766f, 0.5238408446311951f }, { 0.04999832436442375f, 0.0999966710805893f, 0.0999966412782669f }, { 0.43869760632514954, -0.027426747605204582, 0.025848407298326492, 0.8978442549705505 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.39689722657203674f, 0.015331660397350788f, 0.5556508898735046f }, { 0.17599159479141235f, 0.35198312997817993f, 0.3519833981990814f }, { 0.950188398361206, -0.040108662098646164, 0.3047522008419037, -0.051571570336818695 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.23062293231487274f, -0.5142990946769714f, 0.13602864742279053f }, { 0.08318581432104111f, 0.1663716435432434f, 0.1663716733455658f }, { 0.5250418186187744, 0.18281859159469604, 0.5955315828323364, -0.5798713564872742 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ -0.2627076804637909f, 0.4454621374607086f, -0.24406006932258606f }, { 0.08318579196929932f, 0.1663716435432434f, 0.16637162864208221f }, { 0.41131865978240967, -0.27330881357192993, -0.8023005127906799, -0.3353104293346405 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));
		planete2->addElement(new Scenery(Transform<>::BlenderToCookie({ 0.42955711483955383f, 0.3114015460014343f, -0.2056649923324585f }, { 0.08318579941987991f, 0.16637162864208221f, 0.16637156903743744f }, { 0.4183803200721741, 0.09335704147815704, 0.8272531032562256, 0.36317312717437744 }), "graphics/meshs/stalagmite.obj", L"graphics/textureDDS/stalagmiteTexture.dds"));

		Teleport *teleporteur1 = new Teleport(Transform<>::BlenderToCookie({ 0.08071883022785187f, 0.0829712525010109f, 0.07322761416435242f }, { 0.2999899983406067f, 0.2999902367591858f, 0.11999598145484924f }, { 0.8121207356452942, -0.5750795602798462, -0.04842349886894226, -0.08601487427949905 }), "graphics/meshs/teleporteur.obj", L"graphics/textureDDS/teleporteurTexture.dds");
		planete2->addTeleport(teleporteur1);

		teleporteur1->linkedTeleport = nullptr;
		
	}

	scenario.gravityGenerators.push_back(planete2);

	/*scenario.gravityGenerators.push_back(new Planet(Transform<>({ 3000.0f, 0.0f, 3000.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -15.0f, true, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 2000.0f, 1320.0f, 0.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
									      -5.0f, true, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 1200.0f, 1200.0f, 0.0f }, { 100.0f, 100.0f, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -50.f, true, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 2000.0f, 0.0f, 7000.0f }, { 5000.0f, 5000.0f, 5000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -30.0f, true, "newPlanet.obj", L"textureNewPlanet.dds"));*/

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 0.0f, 105.0f, 0.0f }, { 20.0f, 10.0f, 30.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), 
										   -9.81f, false, "graphics/meshs/cargo.obj", L"graphics/textureDDS/cargoTexture.dds"));

	scenario.vehicle = new Vehicle(Transform<>({ 0.0f, 107.0f, 0.0f }, { 4.0f, 1.6f, 4.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), "graphics/meshs/spaceship.obj", L"graphics/textureDDS/spaceshipTexture.dds");
	
	{
		/*Teleport* teleporteur1 = new Teleport(Transform<>({ 0.0f, 107.0f, 5.0f }, { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), "graphics/meshs/spaceship.obj", L"graphics/textureDDS/spaceshipTexture.dds");
		planete2->addTeleport(teleporteur1);

		Teleport *teleporteur2 = new Teleport(Transform<>({ 0.0f, 107.0f, 100.0f }, { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), "graphics/meshs/spaceship.obj", L"graphics/textureDDS/spaceshipTexture.dds");
		planete2->addTeleport(teleporteur2);

		auto T1 = teleporteur1->initialTransform;
		T1.SetPosition({ 10.0f, 107.0f, 30.0f });
		
		auto T2 = teleporteur1->initialTransform;
		T2.SetPosition({ -10.0f, 107.0f, 60.0f });

		auto T3 = teleporteur1->initialTransform;
		T3.SetPosition({ 2.0f, 22.0f, 95.59f });
		
		teleporteur1->linkTo(teleporteur2, { T1, T2, T3 });
		teleporteur2->linkTo(teleporteur1, { T3, T2, T1 });*/
	}

	return scenario;
}
