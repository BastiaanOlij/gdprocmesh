#ifndef GD_PROC_NODE_H
#define GD_PROC_NODE_H

#include <Godot.hpp>
#include <Resource.hpp>
#include <ArrayMesh.hpp>
#include <Material.hpp>

namespace godot {

class GDProcNode : public Resource {
	GODOT_CLASS(GDProcNode, Resource)

public:
	enum ProcessStatus {
		PROCESS_STATUS_PENDING,
		PROCESS_STATUS_INPROGRESS,
		PROCESS_STATUS_UNCHANGED,
		PROCESS_STATUS_CHANGED
	};

private:
	ProcessStatus status; // process status updated as we process our nodes
	Vector2	position; // position in our graph, for editing

protected:
	bool must_update; // we set this to true if one of our properties/settings has changed

public:
	static void _register_methods(); // register our methods and properties and signals

	ProcessStatus get_status() const; // get the current process status
	void set_status(ProcessStatus p_status); // change the process status

	virtual String get_type_name();

	void _init(); // our initializer called by Godot

	virtual bool update(bool p_inputs_updated, const Array &p_inputs); // checks if our node has to be updated and if so, applies our calculations

	virtual int get_input_connector_count() const;
	virtual Variant::Type get_input_connector_type(int p_idx) const;
	virtual const String get_input_connector_name(int p_idx) const;

	virtual int get_output_connector_count() const;
	virtual Variant::Type get_output_connector_type(int p_idx) const;
	virtual const String get_output_connector_name(int p_idx) const;

	// get our actual output for a connector, this should only be called after update has run!
	virtual const Variant get_output(int p_idx) const;

	Vector2 get_position() const;
	void set_position(Vector2 p_pos);
};

}

#endif /* !GD_PROC_NODE_H */
