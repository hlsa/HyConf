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
#include "consts.hpp"

namespace gazebo
{
  class Controller : public ModelPlugin
  {

    int warm_up_counter = 0;


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
      public: virtual void Reset() {
        this->b->theta = 0.0;
        this->b->aspeed = 0.0;
        this->b->dist = 0.0;
        this->b->lspeed = 0.0;     
      }

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
        this->p->left->SetEffortLimit(0,-1);

        this->p->right = _model->GetJoint("right_wheel_hinge");
        printf("assigned right joint\n");
        this->p->right->SetEffortLimit(0,-1);

        // this->p->left->SetEffortLimit(0,1);
        // this->p->right->SetEffortLimit(0,1);

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

        this->p->jointController = this->model->GetJointController();

        this->model->GetJointController()->SetVelocityPID(
          this->p->left->GetScopedName(), common::PID(VELOCITY_P,VELOCITY_I,VELOCITY_D)
        );

        this->model->GetJointController()->SetVelocityPID(
          this->p->right->GetScopedName(), common::PID(VELOCITY_P,VELOCITY_I,VELOCITY_D)
        );

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
        if (warm_up_counter < WARM_UP) {
          printf(".");
          warm_up_counter++;
          // trying to callibrate the accelerometer
          p->base_lX += acc_l.X;
          p->base_lY += acc_l.Y;
          p->base_lZ += acc_l.Z;
          p->base_rX += acc_r.X;
          p->base_rY += acc_r.Y;
          p->base_rZ += acc_r.Z;
          return;
        }
        if (warm_up_counter == WARM_UP) {
          warm_up_counter++;
          p->base_lX /= WARM_UP;
          p->base_lY /= WARM_UP;
          p->base_lZ /= WARM_UP;
          p->base_rX /= WARM_UP;
          p->base_rY /= WARM_UP;
          p->base_rZ /= WARM_UP;
          printf("\n");
          printf("base: (%f,%f,%f),(%f,%f,%f)\n\n",p->base_lX,p->base_lY,p->base_lZ,p->base_rX,p->base_rY,p->base_rZ);
          return;
        }        
        
        sim->step();
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
