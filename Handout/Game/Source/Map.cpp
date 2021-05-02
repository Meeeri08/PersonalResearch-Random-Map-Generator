#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name.create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++) {
			height_map[i][j] = 0;
		}

	return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false)
		return;

	ListItem<MapLayer*>* item = data.layers.start;

	for (; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.GetProperty("Nodraw") != 0)
			continue;

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = MapToWorld(x, y);

					app->render->DrawTexture(tileset->texture, pos.x, pos.y, &r);
				}
			}
		}
	}
}

int Properties::GetProperty(const char* value, int default_value) const
{
	ListItem<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}
// Translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * 16;
	ret.y = y * 16;

	return ret;
}

// Add orthographic world to map coordinates
iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float halfWidth = data.tileWidth * 0.5f;
		float halfHeight = data.tileHeight * 0.5f;
		ret.x = int((x / halfWidth + y / halfHeight) / 2);
		ret.y = int((y / halfHeight - (x / halfWidth)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}


// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));
	
	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	if (!active)
		return true;
	LOG("Unloading map");

	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		app->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	
	// Clean up the pugui tree
	mapFile.reset();
	active = false;
	return true;
}

// Load new map
bool Map::Load(const char* filenameGame)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filenameGame);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filenameGame, result.description());
		ret = false;
	}

	// Load general info
	if(ret == true)
	{
		ret = LoadMap();
	}

	// remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.add(set);
	}
	ret = true;

	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.add(lay);

		pugi::xml_node propertiesNode;
		for (propertiesNode = layer.child("properties"); propertiesNode && ret; propertiesNode = propertiesNode.next_sibling("properties"))
		{
			Properties* property = new Properties();

			ret = LoadProperties(propertiesNode, *property);
			
			lay->properties = *property;
		}
	}

	if(ret == true)
	{
		
		LOG("Successfully parsed map XML file: %s", filenameGame);
		LOG("Width: %d	Hight: %d", data.width, data.height);
		LOG("TileWidth: %d	TileHight: %d", data.tileWidth, data.tileHeight);
		for (int i = 0; i < data.tilesets.count(); i++)
		{
			LOG("TileSet ----");
			LOG("Name: %s	FirstGid: %d", data.tilesets.At(i)->data->name.GetString(), data.tilesets.At(i)->data->firstgid);
			LOG("Tile width: %d", data.tilesets.At(i)->data->tileWidth);
			LOG("Tile Height: %d", data.tilesets.At(i)->data->tileHeight);
			LOG("Spacing: %d", data.tilesets.At(i)->data->spacing);
			LOG("Margin: %d", data.tilesets.At(i)->data->margin);
			LOG("NumTilesWidth: %d", data.tilesets.At(i)->data->numTilesWidth);
			LOG("NumTilesHeight: %d", data.tilesets.At(i)->data->numTilesHeight);
		}
		ListItem<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	mapLoaded = ret;

	return ret;
}

// Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int(0);
		data.height = map.attribute("height").as_int(0);
		data.tileWidth = map.attribute("tilewidth").as_int(0);
		data.tileHeight = map.attribute("tileheight").as_int(0);
		

		if (strcmp(map.attribute("orientation").as_string("MAPTYPE_UNKNOWN"), "orthogonal")==0)data.type = MAPTYPE_ORTHOGONAL;
		else if (strcmp(map.attribute("orientation").as_string("MAPTYPE_UNKNOWN"), "isometric")==0)data.type = MAPTYPE_ISOMETRIC;
		else if (strcmp(map.attribute("orientation").as_string("MAPTYPE_UNKNOWN"), "staggered")==0)data.type = MAPTYPE_STAGGERED;
	}

	return ret;
}

// Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;
	
	set->name = tilesetNode.attribute("name").as_string("");
	set->firstgid = tilesetNode.attribute("firstgid").as_int(0);
	set->tileWidth = tilesetNode.attribute("tilewidth").as_int(0);
	set->tileHeight = tilesetNode.attribute("tileheight").as_int(0);
	set->spacing = tilesetNode.attribute("spacing").as_int(0);
	set->margin = tilesetNode.attribute("margin").as_int(0);
	pugi::xml_node offset = tilesetNode.child("tileoffset");

	return ret;
}

// Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tilesetNode.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string("")));
		set->texWidth = image.attribute("width").as_int(0);
		set->texHeight = image.attribute("height").as_int(0);
	}

	return ret;
}

// Load a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string("");
	layer->width = node.attribute("width").as_int(0);
	layer->height = node.attribute("height").as_int(0);
	layer->mapHeight = data.height* data.tileWidth;
	layer->mapWidth = data.width * data.tileWidth;
	int size = layer->width * layer->height;
	layer->data = new uint[size];
	pugi::xml_node tile = node.child("data").child("tile");
	for (int i = 0; i < size; i++)
	{
		layer->data[i] = tile.attribute("gid").as_uint(0);
		tile = tile.next_sibling("tile");
	}

	return ret;
}

// Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;
	pugi::xml_node propertyNode = node.child("property");

	
	for (propertyNode; propertyNode && ret; propertyNode = propertyNode.next_sibling("property"))
	{
		Properties::Property *propertyID = new Properties::Property();
		propertyID->name = propertyNode.attribute("name").as_string("");
		propertyID->value = propertyNode.attribute("value").as_int(0);
		properties.list.add(propertyID);
	}
	
	return ret;
}
bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* item;
	item = data.layers.start;

	for (item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.GetProperty("Nodraw", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tileId = layer->Get(x, y);
				TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tileId - tileset->firstgid) > 0 ? 0 : 1;
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}

void Map::DrawProceduralMap()
{

	float scale = 0.1f;

	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++) {
			iPoint pos = MapToWorld(x, y);
			
			float value = height_map[x][y];
				
				if (value > 0 && value < 0.2) app->render->DrawTexture(app->scene->forestTex,pos.x, pos.y, NULL, scale);
				else if (value > 0.2 && value < 0.4) app->render->DrawTexture(app->scene->grassTex, pos.x, pos.y, NULL, scale);
				else if (value > 0.4 && value < 0.6)  app->render->DrawTexture(app->scene->sandTex, pos.x, pos.y, NULL, scale);
				else if (value > 0.6 && value < 1)  app->render->DrawTexture(app->scene->waterTex, pos.x, pos.y, NULL, scale);

		}
	}
}