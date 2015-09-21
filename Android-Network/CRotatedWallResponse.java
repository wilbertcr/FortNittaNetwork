package game.network;

public class CRotatedWallResponse extends CPointerLocationResponse {

    CRotatedWallResponse(int time_step, short player_id, int x, int y) {
        super(time_step, player_id, x, y);
    }

    @Override
    //Modify to add game class type as input and implementation.
    public void process() {
        if(CNetworkConstants.TESTING){
            System.out.println("Action: "+CNetworkConstants.ROTATED_WALL+"Received Response.");
        }
    }
}
