#Yangguang He  50947171
from icsp3_class import *
def user_interaction():
    '''
    use userinputs to make a url and turn it into json format, show the outputs followed by user'instructions
    '''
    number1=number_of_cities()#numbers of cities that user inputs
    city_list=a_list(number1)#a list of cities that user inputs
    number2=lines_of_outputs()#number of outputs that user inputs
    the_outputs=a_list(number2)# a list of instrctions that user inputs, such as LATLONG and STEPS.
    diction=get_json_result(build_url(city_list))
    Duck_list=[]
   
    if len(diction['info']['messages'])!=0:
        print('NO ROUTE FOUND')
        return
    else:
        for i in the_outputs:
            if i=='LATLONG':
                Duck_list.append(Latlong(diction))
            if i=='STEPS':
                Duck_list.append(Direction(diction))
            if i=='TOTALTIME':
                Duck_list.append(Time(diction))
            if i=='TOTALDISTANCE':
                Duck_list.append(Distance(diction))
            if i=='ELEVATION':
                Duck_list.append(Elevations(diction))
        for i in Duck_list:
            Ducktype(i)# most important--duck typing
    print('Directions Courtesy of MapQuest; Map Data Copyright OpenStreetMap Contributors')
if __name__ == '__main__':
    user_interaction()
                
                
        
        
    
