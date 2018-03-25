package central.com;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class SpotifyGetAlbumTracks implements Runnable {
    private String response = "no response received";
    private String albumsEndpoint = "https://api.spotify.com/v1/albums/";
    private String albumId = "";
    private String token = "";

    public SpotifyGetAlbumTracks (String token) {
        this.token = token;
    }

    public void setAlbumId(String albumId) {
        this.albumId = albumId;
    }

    @Override
    public void run() {
        if (token == "") {
            this.response = "no token";
        } else {
            try {
                URL url = new URL(albumsEndpoint + albumId + "/tracks");
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                conn.setRequestProperty("Authorization", "Bearer " + this.token);
                try {
                    InputStream is = new BufferedInputStream(conn.getInputStream());
                    int ch;
                    StringBuilder sb = new StringBuilder();
                    while ((ch = is.read()) != -1) {
                        sb.append((char) ch);
                    }
                    this.response = sb.toString();
                } finally {
                    conn.disconnect();
                }
            } catch (IOException e) {
                System.out.println("there was an error " + e);
            }
        }
    }

    public String getResponse() {
        return this.response;
    }
}

