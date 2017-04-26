

class Transform;
class Object;
class Camera;

class Player
{
private:
  Transform * m_transform;
  Object * m_object;
  Camera * m_camera;

public:
  Player(Object * obj, Camera * cam);
  ~Player();

  void update(float delta);
};

