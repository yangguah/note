#Yangguang He 50947171
from ics32_p3_url import*
app_key='key=qMOCjXhsGiGmzse95ATOuxWgJiAcNVMn'
base_map_url='http://open.mapquestapi.com/elevation/v1/profile?'
# the next are 5 classes 
class Time:
    '''
    use the dictionary based on json to get the total time
    '''
    def __init__(self,json):
        self._json=json
    def get_result(self)->str:
        print ('{:}  {:}'.format('TOTAL TIME:',round(self._json['route']['time']/60))+' '+'minutes')
        print('\n')
def a_lng(num:int)->str:
    ''' turn a number into a longitude'''
    if num>0:
        return '{:0.2f} '.format(num)+'E'
    if num<0:
        return '{:0.2f} '.format(abs(num))+'W'
    if num==0:
        return str(0)
def a_lat(num:int)->str:
    '''turn a number into a latitude'''
    if num>0:
        return '{:0.2f} '.format(num)+'N'
    if num<0:
        return '{:0.2f} '.format(abs(num))+'S'
    if num==0:
        return str(0)
        


class Distance:
    '''
    use the dictionary based on json to get the total distance
    '''
    def get_result(self)->str:
        print('{:}  {:}'.format('TOTAL DISTANCE:',round(self._json['route']['distance']))+' '+'miles')
        print('\n')
    def __init__(self,json):
        self._json=json
class Latlong:
    '''
    use the dictionary based on json to get the longitude and laitude
    '''
    def __init__(self,json):
        self._json=json
    def get_result(self)->str:
        print('LATLONGS')
        get_list=self._json['route']["locations"]
        for i in get_list:
            lng=i['latLng']["lng"]
            lat=i['latLng']["lat"]
            print(a_lat(lat)+' '+a_lng(lng))
        print('\n')

        
            
class Direction:
    '''
    use the dictionary based on json to get the directions
    '''
    def __init__(self,json):
        self._json=json
    def get_result(self):
        text=''
        get_list=self._json['route']["legs"]
        for i in get_list:
            for m in i['maneuvers']:
                text+=m['narrative']
                text+='\n'
        print('DIRECTIONS')
        print(text)
        print('\n')

class Elevations:
    '''
    use the dictionary based on json to get the elevation
    '''
    def __init__(self,json):
        self._json=json
    def url(self):
        '''
        use the longitude and latitude to get a url
        '''
        text=[]
        the_url=[]
        get_list=self._json['route']["locations"]
        for i in get_list:
            text.append(str(i['latLng']["lat"])+','+str(i['latLng']["lng"]))
        for i in text:
            the_url.append(base_map_url+app_key+'&unit=f'+'&latLngCollection='+i)
        return the_url
    def get_result(self):
        '''
        turn the url into a json format and get the elevation
        '''
        a_call_list=self.url()
        print('ELEVATIONS')
        for i in a_call_list:
            json=get_json_result(i)
            for i in json["elevationProfile"]:
                print(round(i['height']))
        print('\n')


        
        
def Ducktype(x:'class'):
    ''' this is a duck typing'''
    x.get_result()

        
    
            
            
        
        
        



        
    
