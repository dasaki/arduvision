//----------------
// adapted from code found here:
// http://trandi.wordpress.com/2011/05/16/kalman-filter-simplified-version/
//
public class simpleKalman {
    private double Q = 0.0001;
    private double R = 0.00025;
    private double P = 1, X = 0, K;
     
    private void measurementUpdate(){
        K = (P + Q) / (P + Q + R);
        P = R * (P + Q) / (R + P + Q);
    }
     
    public double update(double measurement){
        measurementUpdate();
        double result = X + (measurement - X) * K;
        X = result;
         
        return result;
    }
}
//---------------

