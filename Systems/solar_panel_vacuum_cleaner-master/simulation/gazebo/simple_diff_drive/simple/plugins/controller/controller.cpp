#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>

namespace gazebo
{
  class Controller : public ModelPlugin
  {
    public:
      Controller() : ModelPlugin()
      {
      }

    public:
      void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
      {
        std::cerr << "\nThe controller plugin is attached to model[" <<
        _model->GetName() << "]\n";

        this->model = _model;

        this->left = _model->GetJoint("left_wheel_hinge");
        this->right = _model->GetJoint("right_wheel_hinge");

        // this->left->SetForce(0,1.0);
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          std::bind(&Controller::OnUpdate,this)
        );
      }

      void OnUpdate()
      {
        std::cerr << "Step\n";
        this->left->SetForce(1,1.0);
        this->right->SetForce(1,1.0);
        
        // this->model->SetLinearVel(ignition::math::Vector3d(.3, 0, 0));
      }

    private: 
        physics::ModelPtr model;
        physics::JointPtr left, right;
        event::ConnectionPtr updateConnection;
  };

  GZ_REGISTER_MODEL_PLUGIN(Controller)
} // namespace gazebo
