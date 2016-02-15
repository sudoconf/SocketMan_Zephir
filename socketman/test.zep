namespace Socketman;

class Test{
	public function say(string !name="jack"){
		let name = " kkkkkppppp";
		var a1 , a2;
		let a1 = null;
		let a2 = null;
		explode("\n" , name);
		echo "Hello" . name;
	}

	public function on(string !str , callback , parameters){
	var instance = null;
	    if typeof callback == "object" {
            if callback instanceof \Closure {
                if typeof parameters == "array" {
                    let instance = call_user_func_array(callback, parameters);
                } else {
                    let instance = call_user_func(callback);
                }
            } else {
                let instance = callback;
            }
        }
	}
}