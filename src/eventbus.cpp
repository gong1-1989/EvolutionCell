#include "eventbus.h"
EventBus* EventBus::m_ins=nullptr;
EventBus::EventBus(QObject *parent)
    : QObject{parent}
{}

EventBus *EventBus::getInstance(){
    if(m_ins==nullptr) m_ins=new EventBus();
    return m_ins;
}

void EventBus::destroyInstance(){
    if(m_ins){
        delete m_ins;
        m_ins=nullptr;
    }
}

void EventBus::dispatchEvent(const QString &evtName, const QVariant &data){
    emit globalEvent(evtName,data);
}