#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/sensors/sensors.hh>
#include <ignition/math/Vector3.hh>
#include <typeinfo>
#include "VacuumCleanerPlatform.hpp"
#include "VacuumCleanerBehaviour.hpp"
#include "Simulation.hpp"
#include <iostream>

namespace gazebo
{
  class Controller : public ModelPlugin
  {

    int warm_up_counter = 0;
    const int WARM_UP = 100;

    public:
      Controller() : ModelPlugin()
      {
      }

      virtual ~Controller() {
        delete this->p;
        delete this->b;
        delete this->sim;
      }

    public:
      void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
      {
        std::cerr << "\nThe controller plugin is attached to model[" <<
        _model->GetName() << "]\n";

        this->model = _model;

        // /* initialised platform, behaviour and simulation */
        this->p =  new VacuumCleanerPlatform();
        printf("created platform\n");

        this->p->left = _model->GetJoint("left_wheel_hinge");
        printf("assigned left joint\n");
        this->p->left->SetEffortLimit(0,0.2);

        this->p->right = _model->GetJoint("right_wheel_hinge");
        printf("assigned right joint\n");
        this->p->right->SetEffortLimit(0,0.2);

        // this->left->SetEffortLimit(0,0.2);
        // this->right->SetEffortLimit(0,0.2);

        sensors::SensorPtr genSensor = sensors::get_sensor("acc_left");
        this->p->acc_left = std::dynamic_pointer_cast<sensors::ImuSensor>(genSensor);
        printf("assigned accelerator left\n");

        genSensor = sensors::get_sensor("acc_right");
        this->p->acc_right = std::dynamic_pointer_cast<sensors::ImuSensor>(genSensor);
        printf("assigned accelerator right\n");

        genSensor = sensors::get_sensor("ultrasonic");
        this->p->ultrasonic_sensor = std::dynamic_pointer_cast<sensors::RaySensor>(genSensor);
        printf("assigned ultrasonic\n");

        this->b = new VacuumCleanerBehaviour(p);
        printf("created behaviours\n");

        this->sim = new Simulation((Platform *)this->p, (Behaviour *)this->b);
        printf("created simulation\n");

        // /* setup components */
        // p->ultrasonicSetup();
        // p->leftAccSetup();
        // p->rightAccSetup();
        // p->batteryLevelSetup();
        // p->chargingSetup();

        // printf("%s\n", typeid(this->ultrasonic).name());

        // this->pid_left = common::PID(2,0,0);
        // this->pid_right = common::PID(2,0,0);
        // this->model->GetJointController()->SetVelocityPID(
        //   this->left->GetScopedName(), this->pid_left
        // );
        // this->model->GetJointController()->SetVelocityPID(
        //   this->right->GetScopedName(), this->pid_right
        // );

        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          std::bind(&Controller::OnUpdate,this)
        );

        // this->updateConnection = event::Events::ConnectStep(
        //   std::bind(&Controller::OnUpdate,this)
        // );

        if (this->p->ultrasonic_sensor == nullptr || this->p->left == nullptr || this->p->right == nullptr ||
            this->p->acc_left == nullptr || this->p->acc_right == nullptr
        ) {
          printf("Some of the sensors and acuators are NULL\n");
          exit(-1);
        }

      }

      void OnUpdate()
      {
        bool acc_l_event = p->acc_l_event;
        struct TripleAxis acc_l = p->acc_l;

        bool acc_r_event = p->acc_r_event;
        struct TripleAxis acc_r = p->acc_r;
        if (warm_up_counter == 0)
          printf("calibrating: ");
        while (warm_up_counter < WARM_UP) {
          printf(".");
          warm_up_counter++;
          // trying to callibraate the accelerometer
          p->base_lX += acc_l.X;
          p->base_lY += acc_l.Y;
          p->base_lZ += acc_l.Z;
          p->base_rX += acc_r.X;
          p->base_rY += acc_r.Y;
          p->base_rZ += acc_r.Z;
        }
        p->base_lX /= WARM_UP;
        p->base_lY /= WARM_UP;
        p->base_lZ /= WARM_UP;
        p->base_rX /= WARM_UP;
        p->base_rY /= WARM_UP;
        p->base_rZ /= WARM_UP;
        
        printf("\n");
        printf("base: (%.2f,%.2f,%.2f),(%.2f,%.2f,%.2f)\n\n",p->base_lX,p->base_lY,p->base_lZ,p->base_rX,p->base_rY,p->base_rZ);
        
        sim->step();

        // ignition::math::Vector3d lacc = this->p->acc_left->LinearAcceleration();
        // ignition::math::Vector3d racc = this->p->acc_right->LinearAcceleration();

        // printf("got accelerations\n");

        // printf("left  acc: %f %f %f\n", lacc.X(), lacc.Y(), lacc.Z());
        // printf("right acc: %f %f %f\n", racc.X(), racc.Y(), racc.Z());

        // std::cout << this->model->WorldLinearAccel() << std::endl;

        // double range = this->p->ultrasonic->Range(0);
        // printf("range: %f\n", range);

        // this->p->left->SetForce(0,-0.2);
        // this->p->right->SetForce(0,0.2);

        // if (range > 0.3) {
        //   // this->left->SetForce(0,-0.3);
        //   // this->right->SetForce(0,-0.3);

        //   this->model->GetJointController()->SetVelocityTarget(
        //     this->left->GetScopedName(), 0.0
        //   );
        //   this->model->GetJointController()->SetVelocityTarget(
        //     this->right->GetScopedName(), 0.0
        //   );
        // } else {
        //   // this->left->SetForce(0,0.1);
        //   // this->right->SetForce(0,0.1);

        //   this->model->GetJointController()->SetVelocityTarget(
        //     this->left->GetScopedName(), 0.5
        //   );
        //   this->model->GetJointController()->SetVelocityTarget(
        //     this->right->GetScopedName(), 0.5
        //   );
        // }

        // double lf = this->left->GetForce(0);
        // printf("left force: %f\n", lf);


        
        // this->model->SetLinearVel(ignition::math::Vector3d(.3, 0, 0));
      }

    private: 
        VacuumCleanerPlatform *p;
        VacuumCleanerBehaviour *b;
        Simulation *sim;
        physics::ModelPtr model;
        event::ConnectionPtr updateConnection;
        
        // common::PID pid_left, pid_right;
        
  };

  GZ_REGISTER_MODEL_PLUGIN(Controller)
} // namespace gazebo
