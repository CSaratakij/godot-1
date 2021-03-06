#ifndef RESOURCE_IMPORT_H
#define RESOURCE_IMPORT_H


#include "io/resource_loader.h"
class ResourceImporter;

class ResourceFormatImporter : public ResourceFormatLoader {

	struct PathAndType {
		String path;
		String type;
	};


	Error _get_path_and_type(const String& p_path,PathAndType & r_path_and_type) const;

	static ResourceFormatImporter *singleton;

	Set< Ref<ResourceImporter> > importers;
public:

	static ResourceFormatImporter *get_singleton() { return singleton; }
	virtual RES load(const String &p_path,const String& p_original_path="",Error *r_error=NULL);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual void get_recognized_extensions_for_type(const String& p_type,List<String> *p_extensions) const;
	virtual bool recognize_path(const String& p_path,const String& p_for_type=String()) const;
	virtual bool handles_type(const String& p_type) const;
	virtual String get_resource_type(const String &p_path) const;
	virtual void get_dependencies(const String& p_path,List<String> *p_dependencies,bool p_add_types=false);

	virtual bool can_be_imported(const String& p_path) const;


	void add_importer(const Ref<ResourceImporter>& p_importer) { importers.insert(p_importer); }
	Ref<ResourceImporter> get_importer_by_name(const String& p_name);
	Ref<ResourceImporter> get_importer_by_extension(const String& p_extension);
	void get_importers_for_extension(const String& p_extension,List<Ref<ResourceImporter> > *r_importers);

	String get_import_base_path(const String& p_for_file) const;
	ResourceFormatImporter();
};


class ResourceImporter : public Reference {

	GDCLASS(ResourceImporter,Reference)
public:
	virtual String get_importer_name() const=0;
	virtual String get_visible_name() const=0;
	virtual void get_recognized_extensions(List<String> *p_extensions) const=0;
	virtual String get_save_extension() const=0;
	virtual String get_resource_type() const=0;
	virtual float get_priority() const { return 1.0; }

	struct ImportOption {
		PropertyInfo option;
		Variant default_value;

		ImportOption(const PropertyInfo& p_info,const Variant& p_default) { option=p_info; default_value=p_default; }
		ImportOption() {}
	};


	virtual int get_preset_count() const { return 0; }
	virtual String get_preset_name(int p_idx) const { return String(); }

	virtual void get_import_options(List<ImportOption> *r_options,int p_preset=0) const=0;
	virtual bool get_option_visibility(const String& p_option,const Map<StringName,Variant>& p_options) const=0;


	virtual Error import(const String& p_source_file,const String& p_save_path,const Map<StringName,Variant>& p_options,List<String>* r_platform_variants,List<String>* r_gen_files=NULL)=0;

};

#endif // RESOURCE_IMPORT_H
