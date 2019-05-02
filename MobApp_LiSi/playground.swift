import UIKit
import PlaygroundSupport

struct ToDo : Decodable { //nötig für Json lesen
    let userId : Int           //exakt so wie in Json benannt
    let id : Int
    let title : String
    let completed : Bool
}

var todos = [ToDo]()    //Array for our todos

let url = URL(string: "https://jsonplaceholder.typicode.com/todos")! //ohne try catch sonst Absturz
let jsonData = try! Data(contentsOf: url)   //ohne catch, schreiben des Url-Inhalts in jsonData (binary data)

todos = try! JSONDecoder().decode([ToDo].self, from: jsonData) //ohne catch, mit JsonDecder speichern wir jede TODO in unser Array todos aus der jsonData