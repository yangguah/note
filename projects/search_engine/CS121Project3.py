#Team:
#Shengquan Ni 46564157
#Tairan Song 52051826
#Yangguang He 50947171

#for tf of each file  how we compute
# process each file and update it meanwhile
import glob
import sys
import codecs
from lxml import etree
from nltk.stem import WordNetLemmatizer
from nltk.tokenize import word_tokenize
from nltk import pos_tag
from nltk.corpus import stopwords, wordnet
from pymongo import MongoClient
from mongoengine import *
from collections import defaultdict
import re
import math
import json
import urllib
import webbrowser


from kivy.app import App
from kivy.lang import Builder
from kivy.core.text import Label
from kivy.uix.screenmanager import *
from kivy.uix.textinput import *
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.button import Button
from kivy.uix.anchorlayout import AnchorLayout
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.properties import ListProperty
from kivy.core.window import Window
from kivy.uix.gridlayout import GridLayout
from kivy.uix.scrollview import ScrollView
Window.size = (500, 600)


wnl=WordNetLemmatizer()
en_stopwords=set(stopwords.words('english'))
file_count=37497
tag_weight={"h1":4,"h2":3,"h3":2,"strong":7,"title":8}
keyword_maxlen=30

def processFile(filename):# store a dic for a file     this dic store tf for each term
    print "start to process "+filename
    try:
        file=open(filename)
        html=etree.HTML(file.read())
        result=defaultdict(lambda :{})
        if html is not None:
            [i.getparent().remove(i) for i in html.xpath("//script|//style|//img|//video")]
            for i in html.xpath("//*"):
                if i.text:
                    extractInfomation(i.text,i.tag,result)
        return result
    except Exception as e:
        print e
        return defaultdict(lambda :{})


def extractInfomation(str,tag,d):
    for i in word_tokenize(str.lower()):
        keyword=preprocessKeyword(i)[0]
        if isVaildKeyword(keyword):
            if "tf" in d[keyword]:
                d[keyword]["tf"]+=(tag_weight[tag] if tag in tag_weight else 1)
            else:
                d[keyword]["tf"]=(tag_weight[tag] if tag in tag_weight else 1)

def generateIndex(directory):
    table=getTableFromDB("InvertedIndex",True)
    idx=directory.find("*")
    for file in glob.glob(directory):
        filename=file[idx:].replace("\\","/")
        updateDB(table,filename,processFile(file))
#example: i {'token':term,'index':{filename1:{'tf':10},filename2:{'tf':12}}}

def getTableFromDB(tablename,drop_if_exist=False):
    client=MongoClient()
    db=client["CS121"]
    res=db[tablename]
    if drop_if_exist and res.count()!=0:
        print "dropping existing table..."
        raw_input("ARE YOU SURE ABOUT THAT?")
        res.drop()
    return res


def updateDB(table,filename,data_dict):
    for i in data_dict:
        res=table.find_one({"token":i})
        if res:
            table.find_and_modify({"token":i},{"$set":{"index."+filename:data_dict[i]}})
        else:
            table.insert({"token":i,"index":{filename:data_dict[i]}})# what our database look like
#example: i {'token':term,'index':{filename1:{'tf':10},filename2:{'tf':12}}}

def computeTFIDF():
    table=getTableFromDB("InvertedIndex")
    for i in table.find():
              print "calculating word '"+i["token"]+"' ..."
        idf=math.log(file_count/(len(i["index"])),10)
        index=i["index"]
        for j in index:
            index[j]["tf-idf"]=(1+math.log(index[j]["tf"],zZzzzz10))*idf
        table.save(i)
#example: i {'token':term,'index':{filename1:{'tf':10,'tf-idf':24},filename2:{'tf':12,'tf-idf':23}}}



def computeVectorScore2():
    bookkeeping=getTableFromDB("BookKeeping")
    table=getTableFromDB("InvertedIndex")
    rec_list=[]
    for i in table.find():
        rec_list.append(i)
    print "rec_list created!"
    for i in bookkeeping.find():#{'file':'1/12','url':'www.google.com'}
        filename=i["file"]
        print "fetching file '"+filename+"' ..."
        tokens=[j for j in rec_list if filename in j["index"]]
        total=0
        for j in tokens:
            idf=math.log(file_count/float(len(j["index"])),10)
            temp=(1+math.log(j["index"][filename]["tf"],10))
            j["index"][filename]["lfw-idf"]=temp*idf
            total+=(temp*idf)**2
        total=math.sqrt(total)
        for j in tokens:
            j["index"][filename]["norm-lfw-idf"]=j["index"][filename]["lfw-idf"]/total
    table=getTableFromDB("InvertedIndex",True)
    table.insert_many(rec_list)
# i= {'token':term,'index':{filename1:{'tf':10,'tf-idf':24,'lfw-idf':24,'norm-lfw-idf':20},filename2:{'tf':12,'tf-idf':23,'lfw-idf':23,'norm-lfw-idf':3}}}
def computeInfomationEntropy(directory):
    res=defaultdict(lambda :{})
    res_range=[1000,-1000]
    idx=directory.find("*")
    for file in glob.glob(directory):
        filename=file[idx:].replace("\\","/")
        print "start to process "+filename
        try:
            d=defaultdict(int)
            total_word=0.0
            file_handle=open(file)
            html=etree.HTML(file_handle.read())
            result=defaultdict(lambda :{})
            if html is not None:
                [i.getparent().remove(i) for i in html.xpath("//script|//style|//img|//video")]
                for i in html.xpath("//*"):
                    if i.text:
                       for j in word_tokenize(i.text.lower()):
                           keyword=preprocessKeyword(j)[0]
                           if isVaildKeyword(keyword): 
                               t=(tag_weight[i.tag] if i.tag in tag_weight else 1)
                               d[keyword]+=t
                               total_word+=t
                
                infomation_entropy=0
                for i in d:
                    d[i]/=total_word
                    infomation_entropy-=d[i]*math.log(d[i],2)
                res[filename]["info_entropy"]=infomation_entropy
                if infomation_entropy>res_range[1]:
                    res_range[1]=infomation_entropy
                elif infomation_entropy<res_range[0]:
                    res_range[0]=infomation_entropy
        except Exception as e:
            print e
            
    
    dist=res_range[1]-res_range[0]
    for i in res:
        norm=(((res[i]["info_entropy"]-res_range[0])/dist)-0.5)*4
        res[i]["weighted_info_entropy"]=(1/math.sqrt(2*math.pi))*math.exp((-norm**2)/2)
    return res





def tableExists(tablename):
    client=MongoClient()
    db=client["CS121"]
    tb=db[tablename]
    return tb.count()!=0



def importFile(tablename,filename):
    client=MongoClient()
    db=client["CS121"]
    tb=db[tablename]
    if tb.count()!=0:
        print "dropping existing table..."
        tb.drop()
    json_obj=json.load(open(filename))
    for i in json_obj:
        tb.insert({"file":i,"url":json_obj[i]})


def isVaildKeyword(keyword,with_warning=False):
    if re.search('\W', keyword):
        if with_warning: print "keyword is contains non-alphanumeric characters!"
        return False
    elif re.search('^[\d.+\-x/_]+$', keyword):
        if with_warning: print "keyword is all about math!"
        return False
    elif re.search('^_*$',keyword):
        if with_warning: print "keyword only contains '_'!"
        return False
    elif keyword in en_stopwords:
        if with_warning: print "keyword is a stopword!"
        return False
    elif len(keyword)>keyword_maxlen:
        if with_warning: print "keyword is too long!"
        return False
    return True

def preprocessKeyword(keywords):
    return [wnl.lemmatize(i.lower()) for i in keywords.split()]




def queryEngine(keywords):
    keywords=preprocessKeyword(keywords)
    keywords=[i for i in keywords if isVaildKeyword(i,True)]
    if keywords==[]:
        return "Invaild query"
    index=getTableFromDB("InvertedIndex")
    res=[index.find_one({"token":i}) for i in keywords]
    if res.count(None) == len(res):
        return "Cannot find any\n webpage with given keyword!"
    else:
        bookkeeping=getTableFromDB("BookKeeping")
        ranking_list=cosineScoreRanking(keywords,index)
        size=min(10,len(ranking_list))
        result=[[i[0],i[1],bookkeeping.find_one({"file":i[0]})["url"]] for i in ranking_list[:size]]
        for i in result:
            file=open("D:\\WEBPAGES_RAW\\"+i[0].replace("/","\\"))
            file_str=file.read()
            title_start=file_str.find("<title>")
            if title_start!=-1:
                end=file_str.find("</title>")
                title=file_str[title_start+7:end]
                try:
                    for j in keywords:
                        key_start=title.lower().find(j)
                        if key_start!=-1:
                            title=title[:key_start]+"[b]"+title[key_start:key_start+len(j)]+"[/b]"+title[key_start+len(j):]
                    i.append(title)
                except:
                    i.append("[color=ff0000][ERROR] Title Parse Error[/color]")
            else:
                i.append("[color=ff0000][ERROR] Title Not Found[/color]")
        return result





def cosineScoreRanking(keywords,table):
    keywords_norm_lfw_idf=defaultdict(float)
    records=[table.find_one({"token":i}) for i in keywords] # query 
    for i in keywords:
        keywords_norm_lfw_idf[i]+=1 # term frequency for query 
    total=0
    now=0
    for i in keywords_norm_lfw_idf:
        df=len(records[now]['index']) if records[now] else 1  #  df  how many doc has this term
        idf=math.log(file_count/df,10)
        temp=(1+math.log(keywords_norm_lfw_idf[i],10))
        keywords_norm_lfw_idf[i]=temp*idf
        total+=(temp*idf)**2
        now+=1
    total=math.sqrt(total)
    for i in keywords_norm_lfw_idf:
        keywords_norm_lfw_idf[i]/=total # normalized query i  {term:norm_lfw_idf}
    result=defaultdict(float)
    now=0
    for i in keywords:
        res=records[now]
        if res:# what if it is dirty word # None
            for j in res["index"]:# each file information for the term
                result[j]+=res["index"][j]["norm-lfw-idf"]*keywords_norm_lfw_idf[i]
        now+=1
    result={i:result[i] for i in sorted(result,None,lambda x:result[x],True)[:100]}  #
    doc_info=getTableFromDB("DocumentInfo")
    for k in result:
        result[k]*=doc_info.find_one({"file":k})["weighted_info_entropy"]
    return sorted(result.items(), key=lambda x:x[1],reverse=True)
    

kv_str='''
<ScreenManager>:
    StartScreen
        id: start_screen
    ResultScreen
        id: result_screen

<StartScreen>
    name: 'StartScreen'
    AnchorLayout:
        anchor_x:'center'
        anchor_y:'center'
        size: 300,80
        pos_hint:{'center_x': .5, 'center_y': .5}
        BoxLayout:
            orientation:'vertical'
            size_hint: None,None
            pos_hint:{'center_x': .5, 'center_y': .5}
            Label:
                text:"[b]CS121 Team 39 Search Engine[/b]"
                markup:True
                size:300,40
                font_size:'20sp'
                size_hint: None,None
                pos_hint:{'center_x': .5, 'center_y': .5}
            TextInput:
                id: text_input
                font_size:'20sp'
                multiline: False
                size:300,40
                size_hint: None,None
                pos_hint:{'center_x': .5, 'center_y': .5}
            Button:
                text:'Search'
                size: 150,40
                size_hint: None,None
                pos_hint:{'center_x': .5, 'center_y': .5}
                on_release: root.Search()

<ResultScreen>
    name: 'ResultScreen'
    Button:
        size:50,50
        text: "Back"
        on_release: root.Clear()
        size_hint: None,None
'''

Builder.load_string(kv_str)

class ScreenManager(ScreenManager):
    pass

class StartScreen(Screen):
    def Search(self):
        self.parent.ids.result_screen.ResultUpdate(queryEngine(self.ids.text_input.text))
        self.ids.text_input.text=""
        self.manager.current="ResultScreen"




class ResultScreen(Screen):
    def ResultUpdate(self,result):
        self.added=None
        if type(result)==str:
            to_add=Label(text=result,font_size='30sp',valign ='center',halign="center")
            self.added=to_add
            self.add_widget(to_add)
        else:
            layout = GridLayout(cols=1, spacing=10, size_hint_y=None,row_force_default=True, row_default_height=80)
            layout.bind(minimum_height=layout.setter('height'))

            for i in result:
                box=BoxLayout(orientation='vertical')
                title=Label(text=i[3],markup=True)
                url=Label(text="[ref="+i[2]+"]"+i[2]+"[/ref]",font_size="12sp",markup=True)
                url.on_ref_press=(lambda x: webbrowser.get("C:/Program Files (x86)/Google/Chrome/Application/chrome.exe %s").open(x))
                content=Label(text="Score: "+str(i[1])+"\n"+"File: "+i[0],font_size="12sp")
                box.add_widget(title)
                box.add_widget(url)
                box.add_widget(content)
                layout.add_widget(box)
            sv = ScrollView(size_hint=(1, None), size=(Window.width, Window.height-50))
            sv.add_widget(layout)
            root=AnchorLayout(anchor_x="left",anchor_y="top")
            root.add_widget(sv)
            self.add_widget(root)
            self.added=root
    def Clear(self):
        self.remove_widget(self.added)
        self.manager.current="StartScreen"


class TestApp(App):
    def build(self):
        return ScreenManager()




if __name__=="__main__":
    
    if not tableExists("BookKeeping"):
        importFile("BookKeeping",r"D:\WEBPAGES_RAW\bookkeeping.json")

    #generateIndex(r"D:\WEBPAGES_RAW\*\*")
    #print "index generated! Now computing TF-IDF..."
    #computeTFIDF()
    #print "computed TF-IDF! All changes saved in database!"
    #computeVectorScore()
    #while True:
    #    keywords=raw_input("enter words to query: ")
    #    queryEngine(keywords)
    TestApp().run()
