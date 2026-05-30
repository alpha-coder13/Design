/**
 * implementing obeserver pattern
 * 
 */

#include<vector>
#include<string>

class PublishStrategy;
struct SubscriberConfig{
    std::string eventType;
};

struct PublishPacket{};
class Subscriber {
    protected:
        SubscriberConfig config;
    public:
        virtual SubscriberConfig getConfig() const =0;
        virtual void setConfig() =0;
        virtual void sendUpdate(PublishPacket) const = 0;
};

class PublishStrategy {
    public:
        virtual void sendUpdate(std::vector<Subscriber*>, PublishPacket){};
};

class PointerPublishStrategy : public PublishStrategy{
    public: 
        void sendUpdate(std::vector<Subscriber*> subscriberArray,PublishPacket packet){
            for(auto a : subscriberArray){
                if(a->getConfig().eventType == "pointer"){
                    a->sendUpdate(packet);
                }
            }
        }
};

class KeyPressPublishStrategy : public PublishStrategy{
    public: 
        void sendUpdate(std::vector<Subscriber*> subscriberArray,PublishPacket packet){
            for(auto a : subscriberArray){
                if(a->getConfig().eventType == "KeyPress"){
                    a->sendUpdate(packet);
                }
            }
        }
};

class Publisher{
    protected:
        PublishStrategy* publishStrategy;
        std::vector<Subscriber*> subscribers;
    public:
        virtual void setPublishStrategy(PublishStrategy*) = 0;
        virtual void subscribe(Subscriber*) = 0;
        virtual void sendUpdate(PublishPacket) const = 0;
};

class EventPublisher : public Publisher {
    public:
        void setPublishStrategy(PublishStrategy* publishStrategy){
            this->publishStrategy = publishStrategy;
        } 
        void subscribe(Subscriber* subscriberPtr){
            this->subscribers.push_back(subscriberPtr);
        }

        void sendUpdate(PublishPacket publishPacket) const override{
          this->publishStrategy->sendUpdate(this->subscribers, publishPacket);
        }
};

/**
 * the above design has a flaw where the publisher uses strategy to send responses:
 *  : kind of event bus system but deviates from standard oberserver pattern where the subscribers filter the message not the
 * publisher
 * 
 * : pros ->
 *      1. event bus like architechture like that of kafka
 *      2. dynamic strategy changes resulting in flexible publishing in runtime
 * 
 * : cons -> 
 *      1. deviates from standard oberserver pattern : (the event is broadcasted to all and only will subscribers work ont he events)
 *      2. storing events in various deticated maps would make strategy implemetnation / bus driven model much better but will trade for space.
 */