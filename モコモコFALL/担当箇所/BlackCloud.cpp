#include "BlackCloud.h"


BlackCloud::BlackCloud( ) {
}

BlackCloud::~BlackCloud( ) {
}

void BlackCloud::setStatus( Status status ) {
    _status = status;
}

Vector2 BlackCloud::getPos( ) const {
    return _status.pos;
}

double BlackCloud::getScale( ) const {
    return _status.scale;
}
