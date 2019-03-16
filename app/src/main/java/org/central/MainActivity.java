package org.central;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.Switch;
import android.widget.TextView;

import java.util.LinkedHashMap;

public class MainActivity extends AppCompatActivity {

    private String clientId = "your_client_id";
    private String clientSecret = "your_client_secret";
    private String token = "";
    private String myText = "";
    private boolean ascending = false;
    private String myString = "";
    private String myArtist = "";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("super-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // hooks into the ui elements
        final TextView tv = findViewById(R.id.my_text);
        final ScrollView myScroll = findViewById(R.id.my_scroll);
        Button clearButton = findViewById(R.id.clear);
        final EditText myInput = findViewById(R.id.my_input);
        Switch mySwitch1 = findViewById(R.id.my_switch1);
        final Button myButton = findViewById(R.id.my_button);

        // get a spotify token for requests

        token = getToken(clientId, clientSecret);

        myInput.setText(String.valueOf(myString));
        myInput.clearFocus();

        clearButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                myText = "";
                tv.setText(myText);
            }
        });

        myInput.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {}

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {}

            @Override
            public void afterTextChanged(Editable editable) {
                if (editable.toString().length() > 0) {
                    myArtist = editable.toString().replace(" ", "%20");
                }
            }
        });

        mySwitch1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                ascending = isChecked;
            }
        });

        myButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

/*
                // this is just a hello world thing to test passing types through the native interface
                myText = getStringJNI();
                LinkedHashMap<String, String> myMap = returnLinkedHashMapJNI();
                myText = myMap.get("key1");
*/

/*
                // search artist and get # 0f followers of the first artist in search results - example run dmc gives 531532

                SpotifySearchArtist ssaRequest = new SpotifySearchArtist(token);
                Thread ssaRequestThread = new Thread(ssaRequest);
                ssaRequest.setSearchTerm(myArtist);
                ssaRequestThread.start();
                try { ssaRequestThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }
                String artistSearchJson = ssaRequest.getResponse();
                myText = parseSearchJsonGetFollowersJNI(artistSearchJson);
*/

                // this will return a list of related artists - sorted by popularity
                SpotifySearchArtist ssaRequest = new SpotifySearchArtist(token);
                Thread ssaRequestThread = new Thread(ssaRequest);
                ssaRequest.setSearchTerm(myArtist);
                ssaRequestThread.start();
                try { ssaRequestThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }
                String artistSearchJson = ssaRequest.getResponse();

                // get similar artists
                SpotifyGetArtistSimilarArtists sgasaRequest = new SpotifyGetArtistSimilarArtists(token);
                sgasaRequest.setArtistId(getArtistId(artistSearchJson));

                Thread sgasaRequestThread = new Thread(sgasaRequest);
                sgasaRequestThread.start();
                try { sgasaRequestThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }

                LinkedHashMap<String, Integer> myLHM = relatedArtistsJsonToLinkedHashMapJNI(sgasaRequest.getResponse(), ascending);

                StringBuilder sb = new StringBuilder();

                for (String key : myLHM.keySet()) {
                    String myLine = myLHM.get(key) + " - " + key + "\n";
                    sb.append(myLine);
                }

                myText = sb.toString();

/*
                // this returns the json response from a request for similar artists with thumbnail images etc
                SpotifySearchArtist ssaRequest = new SpotifySearchArtist(token);
                Thread ssaRequestThread = new Thread(ssaRequest);
                ssaRequest.setSearchTerm(myArtist);
                ssaRequestThread.start();
                try { ssaRequestThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }
                String artistSearchJson = ssaRequest.getResponse();

                SpotifyGetArtistSimilarArtists sgasaRequest = new SpotifyGetArtistSimilarArtists(token);
                sgasaRequest.setArtistId(getArtistId(artistSearchJson));

                Thread sgasaRequestThread = new Thread(sgasaRequest);
                sgasaRequestThread.start();
                try { sgasaRequestThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }

                myText += parseDumpJsonJNI(sgasaRequest.getResponse());
*/
/*
                // this returns the response from the artist endpoint using he artistId
                SpotifySearchArtist ssaRequest = new SpotifySearchArtist(token);
                Thread ssaRequestThread = new Thread(ssaRequest);
                ssaRequest.setSearchTerm(myArtist);
                ssaRequestThread.start();
                try { ssaRequestThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }
                String artistSearchJson = ssaRequest.getResponse();

                SpotifyGetArtist artistRequest = new SpotifyGetArtist(token);
                artistRequest.setArtistId(getArtistId(artistSearchJson));
                Thread artistThread = new Thread(artistRequest);
                artistThread.start();
                try { artistThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }

                myText = artistRequest.getResponse();
*/
/*
                // this gives you the list of albums by the searched artist
                SpotifySearchArtist ssaRequest = new SpotifySearchArtist(token);
                Thread ssaRequestThread = new Thread(ssaRequest);
                ssaRequest.setSearchTerm(myArtist);
                ssaRequestThread.start();
                try { ssaRequestThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }
                String artistSearchJson = ssaRequest.getResponse();

                SpotifyGetArtistAlbums artistAlbumsRequest = new SpotifyGetArtistAlbums(token);
                artistAlbumsRequest.setArtistId(getArtistId(artistSearchJson));
                Thread artistThread = new Thread(artistAlbumsRequest);
                artistThread.start();
                try { artistThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }

                String albumsJson = artistAlbumsRequest.getResponse();

                SpotifyGetAlbum albumRequest = new SpotifyGetAlbum(token);
                albumRequest.setAlbumId(parseArtistAlbumsJsonGetAlbumIdJNI(albumsJson));
                Thread albumThread = new Thread(albumRequest);
                albumThread.start();
                try { albumThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }

                myText = albumRequest.getResponse();
*/

/*
                // this returns the first track from the first album from the tracks api endpoint
                SpotifySearchArtist ssaRequest = new SpotifySearchArtist(token);
                Thread ssaRequestThread = new Thread(ssaRequest);
                ssaRequest.setSearchTerm(myArtist);
                ssaRequestThread.start();
                try { ssaRequestThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }
                String artistSearchJson = ssaRequest.getResponse();

                SpotifyGetArtistAlbums artistAlbumsRequest = new SpotifyGetArtistAlbums(token);
                artistAlbumsRequest.setArtistId(getArtistId(artistSearchJson));
                Thread artistThread = new Thread(artistAlbumsRequest);
                artistThread.start();
                try { artistThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }

                String albumsJson = artistAlbumsRequest.getResponse();

                SpotifyGetAlbumTracks albumTracksRequest = new SpotifyGetAlbumTracks(token);
                albumTracksRequest.setAlbumId(parseArtistAlbumsJsonGetAlbumIdJNI(albumsJson));
                Thread albumTracksThread = new Thread(albumTracksRequest);
                albumTracksThread.start();
                try { albumTracksThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }

                String albumTracksJson = albumTracksRequest.getResponse();

                SpotifyGetTrack trackRequest = new SpotifyGetTrack(token);
                trackRequest.setTrackId((parseAlbumTracksJsonGetTrackIdJNI(albumTracksJson)));
                Thread trackThread = new Thread(trackRequest);
                trackThread.start();
                try { trackThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }

                myText = trackRequest.getResponse();
*/
                tv.setText(myText);
            }
        });

        tv.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                myScroll.fullScroll(View.FOCUS_DOWN);
            }
        });
    }

    public String getToken(String clientId, String clientSecret) {
        SpotifyGetToken request = new SpotifyGetToken(clientId, clientSecret);

        Thread requestThread = new Thread(request);
        requestThread.start();
        try { requestThread.join(); } catch (InterruptedException e) { e.printStackTrace(); }
        String response = request.getResponse();

        String myToken = parseTokenJsonGetTokenJNI(response);
        return myToken;
    }

    public String getArtistId(String searchArtistJson) {
        String myArtistId = parseSearchJsonGetArtistIdJNI(searchArtistJson);
        return myArtistId;
    }

    /**
     * native methods in 'super-lib.cpp'
     */

    public native String parseDumpJsonJNI(String myJson);

    public native String parseTokenJsonGetTokenJNI(String myJson);

    public native String parseSearchJsonGetArtistIdJNI(String myJson);

    public native String parseArtistAlbumsJsonGetAlbumIdJNI(String myJson);

    public native String parseAlbumTracksJsonGetTrackIdJNI(String myJson);

    public native String getStringJNI();

    public native LinkedHashMap<String, String> returnLinkedHashMapJNI();

    public native LinkedHashMap<String, Integer> returnIntLinkedHashMapJNI();

    public native LinkedHashMap<String, Integer> relatedArtistsJsonToLinkedHashMapJNI(String myJson, boolean myAsc);

    public native String parseSearchJsonGetFollowersJNI(String myJson);
}