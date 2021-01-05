#ifndef BT_BEHAVIOR_H
#define BT_BEHAVIOR_H

#include <functional>

enum class BehaviorStatus {
  INVALID,
  RUNNING,
  FAILURE,
  SUCCESS
};

class Behavior {
private:
  BehaviorStatus m_status;
  std::function<BehaviorStatus()> m_callback;

public:
  Behavior() : m_status(BehaviorStatus::INVALID) {}
  virtual ~Behavior() {}
  virtual BehavorStatus run() {
    if (m_callback) {
      m_status = callback();
    }
    return m_status;
  }
};

class CompositeBehavior : public Behavior {
private:
  std::vector<std::shared<Behavior>> children;
public:
  const std::vector<std::shared<Behavior>>& getChildren() const {return children;}
  void addChild (Behavior* child) {children.emplace_back(child);}
};

class Selector : public CompositeBehavior {
public:
  virtual bool run() override {
    for (std::shared<Behavior>> child : getChildren()) {  // The generic Selector implementation
      if (child->run()) {  // If one child succeeds, the entire operation run() succeeds.  Failure only results if all children fail.
        return true;
      }
    }
    return false;  // All children failed so the entire run() operation fails.
  }
};

class Sequence : public CompositeBehavior {
public:
  virtual bool run() override {
    for (std::shared<Behavior>> child : getChildren()) {  // The generic Sequence implementation.
      if (!child->run())  // If one child fails, then enter operation run() fails.  Success only results if all children succeed.
        return false;
    }
    return true;  // All children suceeded, so the entire run() operation succeeds.
  }
};

class DecoratorNode : public Node {  // Function is either to transform the result it receives from its child node's status, to terminate the child, or repeat processing of the child, depending on the type of decorator node.
private:
  std::shared<Behavior> m_child;  // Only one child allowed
protected:
  std::shared<Behavior> getChild() const {return m_child;}
public:
  void setChild (const std::shared<Behavior>& newChild) {m_child = newChild;}
};

#endif
