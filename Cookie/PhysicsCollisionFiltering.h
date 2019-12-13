#pragma once

namespace Cookie {

	/*
	Si un objet a comme filter group DEFAULT, il entrera en collision normalement avec tous les autres objets DEFAULT
	S'il a un autre flag dans son mask et qu'il entre en collision avec un objet portant ce flag dans son filter group, on déclenche son callback
	*/

	enum FilterGroup {
		DEFAULT = (1 << 0),
		VEHICULE = (1 << 1),
		PLANET = (1 << 2)
	};
}