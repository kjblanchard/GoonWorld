
int AddBodyToScene(SDL_Rect* bodyRect, int bodyType);
int AddStaticBodyToScene(SDL_Rect* bodySdlRect);
gpBody *GetBodyFromScene(int bodyNum);
int AddForceToBody(int bodyNum, float forceX, float forceY);
int ToggleBodyGravity(int bodyNum, int gravity);
int SetBodyType(int bodyNum, int bodyType);
int IsBodyOnGround(int bodyNum);
int GetOverlapDirection(int bodyNum, int overlapBodyNum);
gpVec GetBodyCoordinates(int bodyRef);
int SetBodyPos(int bodyRef, gpVec pos);
gpVec GetBodyVelocity(int bodyRef);
int SetBodyVelocity(int bodyRef, gpVec velocity);