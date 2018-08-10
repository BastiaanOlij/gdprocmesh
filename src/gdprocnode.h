#ifndef GD_PROC_NODE_H
#define GD_PROC_NODE_H

#include <Godot.hpp>
#include <Resource.hpp>
#include <ArrayMesh.hpp>
#include <Material.hpp>

namespace godot {

class GDProcMesh;

class GDProcNode : public Resource {
	GODOT_CLASS(GDProcNode, Resource)

public:
	enum ProcessStatus {
		PROCESS_STATUS_PENDING, // all nodes are cleared to this before we update our mesh
		PROCESS_STATUS_INPROGRESS, // a node gets this status when we are in the middle of updating it, helps detect cyclic relationships
		PROCESS_STATUS_UNCHANGED, // a node gets this status once we finish updating and find the node unchanged
		PROCESS_STATUS_CHANGED // a node gets this status once we finish updating at its contents has changed
	};

private:
	ProcessStatus status; // process status updated as we process our nodes
	Vector2	position; // position in our graph, for editing
	String node_name; // name of this node, I had issues using the build in name not being saved...

protected:
	bool must_update; // we set this to true if one of our properties/settings has changed

public:
	static void _register_methods(); // register our methods and properties and signals

	ProcessStatus get_status() const; // get the current process status
	void set_status(ProcessStatus p_status); // change the process status

	virtual String get_type_name(); // gets the name we display in the title of the GraphNode

	void set_node_name(const String p_node_name);
	String get_node_name() const;

	void _init(); // our initializer called by Godot
	void _touch(); // marks this node for update

	virtual bool update(bool p_inputs_updated, const Array &p_inputs); // checks if our node has to be updated and if so, applies our calculations

	virtual Variant::Type get_input_property_type() const; // if this is an input property, what is its type?
	virtual void set_input(Variant p_input); // if this is an input property, set its value
	virtual Variant get_input() const; // if this is an input property, get its value

	virtual int get_input_connector_count() const; // returns the number of input connectors this note has
	virtual Variant::Type get_input_connector_type(int p_slot) const; // returns the type of the data expected for this input
	virtual const String get_input_connector_name(int p_slot) const; // returns the name for this input

	virtual const String get_connector_property_name(int p_slot) const; // if we want an editable field for this slot, returns the name of the related property

	virtual int get_output_connector_count() const; // returns the number of output connectors this node has
	virtual Variant::Type get_output_connector_type(int p_slot) const; // returns the type of the data that is output by this output
	virtual const String get_output_connector_name(int p_slot) const; // returns the name for this output

	// get our actual output for a connector, this should only be called after update has run!
	virtual const Variant get_output(int p_slot) const; // returns the output data itself

	Vector2 get_position() const; // get the display position of this node in our graph
	void set_position(Vector2 p_pos); // sets the display position of thisnode in our graph
};

}

#endif /* !GD_PROC_NODE_H */
