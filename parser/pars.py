import xml.etree.ElementTree as ET
import csv
import geopy.distance

file_path = 'map.xml'
parser = ET.XMLParser(encoding='utf-8')
tree = ET.parse(file_path, parser=parser)
root = tree.getroot()

csv_parsed_ways = 'parsed_ways.csv'
csv_parsed_nodes = 'parsed_nodes.csv'
csv_parsed_metadata = 'parsed_metadata.csv'
csv_parsed_transport = 'parsed_transport.csv'

way_index = 0
node_index = 0
transport_index = 1

new_ways = []
new_nodes = []
new_metadata = []
new_transport = []

node_coord = {}

subway_node_list_to_filter = list()
exclusion_list = [3489940106, 2739073043, 3489940106] 

def filter_subway_nodes(nodes) -> list:
    if len(nodes) == 0 :
        #print("Subway list is empty")
        return list()
    
    filtered_list = list()
    for node in nodes:
        to_add = True
        for unique_node in filtered_list:
            if unique_node[1] == node[1]: # filter by name
                to_add = False
                #print("Unique node: {} {} lat {} lon {} replaces -> {} lat {} lon {}".format(
                    #unique_node[0]
                    #, unique_node[1]
                    #, unique_node[2]
                    #, unique_node[3]
                    #, node[0]
                    #, node[2]
                    #, node[3]))
                break
        if to_add:
            filtered_list.append(node)

    return filtered_list


def calculate_distance(p1, p2):
    distance = geopy.distance.Distance(0.0)
    try:
        distance = geopy.distance.geodesic(node_coord[p1],
                                      node_coord[p2])
    except Exception as ex:
        print("No distance: ", ex)
    return distance

def healthcare_to_facility_type(type):
    match type:
        case "pharmacy":
            return 0  
        case "emergency":
            return 1
        case "hospital":
            return 3
        case "trauma":
            return 4
        case "clinic":
            return 5
        case "stomatology":
            return 6
        case "orthodontics":
            return 6
        case "dentist":
            return 6
        case "gynaecology":
            return 7
        case "urology":
            return 8
        case "birthing_centre":
            return 9
        case "general":
            return 10
        case "paediatrics":
            return 10
        case "community":
            return 10
        case "centre":
            return 10
        case "doctor":
            return 10
    return 404

def transport_type_to_speed(type):
    match type:
        case 0:
            return 80.0 # u-bahn
        case 1:
            return 40.0 # bus
        case 2:
            return 45.0 # car
        case 3:
            return 4.5 # pedestrian
    return 1

def transport_type_to_cost(type):
    match type:
        case 0:
            return 8.0
        case 1:
            return 15.0
        case 2:
            return 1.0
        case 3:
            return 1.0
    return 1

def string_to_transport_type(str):
    match str:
        case 'subway':
            return 0
        case 'bus' :
            return 1
        case 'share_taxi':
            return 1
        case 'road':
            return 2
    return None

print("Node parsing...")
for node in root.findall('node'):
    node_id = node.get('id')
    lat = node.get('lat')
    lon = node.get('lon')

    node_coord[node_id] = (lat, lon) 

    name = str()
    street = str()
    street_number = str()
    facility_type = set()

    found_subway = False
    
    for tag in node.findall('tag'):
        key = tag.get('k')
        value = tag.get('v')
        if key == "name:uk":
            name = tag.get('v')
        elif key == "addr:street":
            street = tag.get('v')
        elif key == "addr:housenumber":
            street_number = tag.get('v')
        elif key == "healthcare":
            facility_type.add(healthcare_to_facility_type(tag.get('v')))
        elif key == "healthcare:speciality":
            for type in tag.get('v').split(";"):
                facility_type.add(healthcare_to_facility_type(type))

        if key == "subway" and value == "yes" and name != "":
            node = [node_id, name, lat, lon]
            subway_node_list_to_filter.append(node)
            found_subway = True


    if street.strip != "" and street_number.strip() != "":
        street = street + " " + street_number

    new_metadata.append([node_id, name, facility_type, street, lat, lon])
    new_nodes.append([node_id, node_id])

print("Constructing subway...")
filtered_subway_nodes = filter_subway_nodes(subway_node_list_to_filter)
#print("{} unique stations".format(len(filtered_subway_nodes)))

def name(node):
    return node[1]
filtered_subway_nodes.sort(key=name)

#for subway_node in filtered_subway_nodes:
    #print("Id {} {} lat {} lon {}".format(subway_node[0], subway_node[1], subway_node[2], subway_node[3]))
        

print("Way parsing...")

way_ref_to_id = {}
way_index_to_distance = {}
way_index_to_transport_index = {}
way_indexes =[]
subway_way_ref = []


for way in root.findall('way'):
    way_ref = way.get('id')
    ref_list = [nd.get('ref') for nd in way.findall('nd')]
    way_street = str()

    found_subway = False
    for tag in way.findall('tag'):
        if tag.get('k') == "name:uk":
            way_street = tag.get('v')
        if tag.get('k') == 'railway' and tag.get('v') == 'subway':
            subway_way_ref.append(way_ref)    

    # for i in range(len(ref_list)):
    #     for j in range(len(new_nodes)):
    #         if ref_list[i] == new_nodes[j][0]:
    #             ref_list[i] = node_counter
    #             new_nodes[j][0] = node_counter
    #             new_metadata[j][0] = node_counter
    #             node_counter += 1

    for i in range(len(ref_list) - 1):
        current_node = ref_list[i]
        next_node = ref_list[i + 1]

        distance = calculate_distance(current_node, next_node).meters
        
        way_index_to_distance[way_index] = distance
        new_ways.append([way_index, current_node, next_node, way_street.strip(), distance, 0])
      
        way_indexes.append(way_index)
        way_index += 1

    way_ref_to_id[way_ref] = way_indexes.copy()
    way_indexes.clear()


print("Transport parsing...")

new_transport.append([0, "Pedestrian", [3]])

for relation in root.findall('relation'):
    transport_type_str = str()
    transport_name_str = str()
    for tag in relation.findall('tag'):
        if tag.get('k') == "route":
            transport_type_str = tag.get('v')
        elif tag.get('k') == "name:uk":
            transport_name_str = tag.get('v')

    for member in relation.findall('member'):
        if member.get('type') != 'way':
            continue
        
        way_ref = member.get('ref')
        
        if member.get('type') == 'way':   
            if way_ref_to_id.get(way_ref) is None:
                continue
            way_indexes = way_ref_to_id[way_ref]
            if way_ref in subway_way_ref :
                transport_type_str = 'subway'

        for way_index in way_indexes:
            transport_types = set()
            converted_type = string_to_transport_type(transport_type_str)
            if converted_type != None:
                transport_types.add(converted_type)
                if converted_type != 0:
                    transport_types.add(3) # add pedestrian road type by default
            if way_ref in subway_way_ref:
                transport_types.add(0) # add subway type    
            if len(transport_types) == 0:
                transport_types.add(3)
            
            transport_name = transport_name_str
            new_transport.append([transport_index, transport_name, list(transport_types)])

            way_index_to_transport_index[way_index] = transport_index


            transport_index += 1
        
node_ref_to_new_index = dict()

print("Setting metadata and way indexes...")


#assert len(new_nodes) == len(new_metadata)
#for i in range(len(new_nodes)):
#    new_nodes[i][0] = i
#    new_metadata[i][0] = i

#node_counter = 0
#changed_nodes = set()
print("Setting indexes...")
for i in range(len(new_ways)):
    if way_index_to_transport_index.get(new_ways[i][0]) is not None:
        new_ways[i][-1] = way_index_to_transport_index[new_ways[i][0]]
        #print("new_ways", new_ways[i][-1])

    #for j in range(len(new_nodes)):
    #    #print("new_ways[i][1]", new_ways[i][1])
    #    #print("new_nodes[j][0]", new_nodes[j][0])
    #    #print("changed_nodes", changed_nodes)
    #    if new_ways[i][1] not in changed_nodes and new_ways[i][1] == new_nodes[j][0]:
    #        changed_nodes.add(new_ways[i][1])
    #
    #        new_ways[i][1] = node_counter
    #        new_nodes[j][0] = node_counter
    #        node_counter += 1
    #    if  new_ways[i][2] not in changed_nodes and new_ways[i][2] == new_nodes[j][0]:
    #        changed_nodes.add(new_ways[i][2])
    #
    #        new_ways[i][2] = node_counter
    #        new_nodes[j][0] = node_counter
    #        node_counter += 1

print("Collecting data...")

print("Writing nodes...")
with open(csv_parsed_nodes, 'w', newline='', encoding='utf-8') as csv_file:
    csv_writer = csv.writer(csv_file, delimiter=';')
    csv_writer.writerow(['id', 'meta_id'])
    for id, meta_id in new_nodes: 
        csv_writer.writerow([id, meta_id])

print("Writing ways...")
with open(csv_parsed_ways, 'w', newline='', encoding='utf-8') as csv_file:
    csv_writer = csv.writer(csv_file, delimiter=';')
    csv_writer.writerow(['id', 'from_node', 'to_node', 'street', 'distance', 'transport_id'])
    for id, from_node, to_node, street, distance, transport_id in new_ways:
        csv_writer.writerow([id, from_node, to_node, street, distance, transport_id])

print("Writing metadata...")
with open(csv_parsed_metadata, 'w', newline='', encoding='utf-8') as csv_file:
    csv_writer = csv.writer(csv_file, delimiter=';')
    csv_writer.writerow(['id', 'facility_name', 'address', 'facility_type', 'latitude', 'longitude'])
    for id, name, facility_type, street, lat, lon in new_metadata:
        csv_writer.writerow([id, name, str(street), facility_type, lat, lon])

print("Writing transports...")
with open(csv_parsed_transport, 'w', newline='', encoding='utf-8') as csv_file:
    csv_writer = csv.writer(csv_file, delimiter=';')
    csv_writer.writerow(['id', 'transport_name', 'transport_type'])
    for id, transport_name, transport_type in new_transport:
        csv_writer.writerow([id, transport_name, transport_type])
