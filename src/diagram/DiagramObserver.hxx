#ifndef DIAGRAMOBSERVER_HXX
#define DIAGRAMOBSERVER_HXX

namespace DBricks {

class Diagram;

class DiagramObserver {
public:
    DiagramObserver(Diagram* diagram)
        :m_diagram(diagram)
    {
    }
    
    virtual ~DiagramObserver()
    {
    }
    
    virtual void update() = 0;
protected:
    Diagram* m_diagram;
};

} // namespace DBricks

#endif // DIAGRAMOBSERVER_HXX
