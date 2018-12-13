#Yangguang He 50947171
import urllib.request
import json
import urllib.parse

def number_of_cities()->int:
    '''
    Ask the user to specify how many cities they wanna search,
    the input should be an integer that is equal or bigger than 2
    '''
    while True:
        try:    
            number=int(input())
            if number>=2:
                return number
            else:
                print('number must be equal or bigger than 2')
        except:
            print('ERROR,please inuput a number equal or bigger than 2')


            
def lines_of_outputs()->int:
    '''
    Ask the user to specify how many lines of outputs they wanna get,
    the input should be equal or bigger than 2.
    '''
    while True:
        try:
            number=int(input())
            if number>=1:
                return number
            else:
                print('number must be equal or bigger than 1')
        except:
            print('ERROR,please input a number equal or larger than 1')
def a_list(number:int)->list:
    '''
     use an integer as a parameter and return the list
    '''
    result=[]
    for i in range(number):
        user_input=input()
        result.append(user_input)
    return result

def build_url(a_list:list)->'url':
    '''
    build a url from a list
    '''
    url='http://open.mapquestapi.com/directions/v2/route?'#from=Irvine%2CCA&to=Los+Angeles%2CCA
    query_parameters=[('key', 'qMOCjXhsGiGmzse95ATOuxWgJiAcNVMn')]
    if len(a_list)>=2:
        query_parameters.append(('from',a_list[0]))
        for i in range(1,len(a_list)):
            query_parameters.append(('to',a_list[i]))
    url+=urllib.parse.urlencode(query_parameters)
    return url
print(build_url(['a','b','c','d']))
        

def get_json_result(url:str) -> 'json':
    '''
    This function takes a URL and returns a Python object representing the
    parsed JSON response.
    '''

    response=None
    try:
        response = urllib.request.urlopen(url)
        json_text = response.read().decode(encoding='utf-8')
        return json.loads(json_text)
    except:
        print('MAPQUEST ERROR')
    finally:
        if response != None:
            response.close()
