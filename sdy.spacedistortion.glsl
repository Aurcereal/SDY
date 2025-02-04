// Later make sizes Defines
SpaceOpNode spaceNodes[32];
vec3 distortedPs[6];

void calculatePs(vec3 p) {

    vec3 cp = p;
    int currInsertionIndex = 0;

    for(int i=0; i<32; i++) {
        SpaceOpNode node = spaceNodes[i];
        switch(node.operationType) {
            case SPOP_NOP:
            distortedPs[currInsertionIndex] = cp;
            currInsertionIndex++;
            if(currInsertionIndex >= 6) break; // Later have a branch count don't use static size
            cp = p;
            break;

            case SPOP_TWIST:
            // TODO: implement all of these using param data
            break;

            case SPOP_MIRROR:

            break;

            case SPOP_REPEAT:

            break;

            case SPOP_PINCH:

            break;
        }
    }
}

