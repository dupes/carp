CREATE TABLE "frames" ("id" INTEGER PRIMARY KEY  NOT NULL ,"video_id" INTEGER,"num_objects" INTEGER,"original_image" BLOB,"bw_image" BLOB,"contour_image" BLOB,"reduced_image" BLOB,"number" INTEGER);
CREATE TABLE "objects" ("id" INTEGER PRIMARY KEY  NOT NULL ,"frame_id" INTEGER,"object_image" BLOB,"contour" BLOB,"x" INTEGER,"y" INTEGER,"height" INTEGER,"width" INTEGER,"number" INTEGER,"label" TEXT,"verified" INTEGER,"area" REAL,"cluster_id" INTEGER DEFAULT (null) );
CREATE TABLE videos (
    "id"INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    "name" TEXT,
    "rows" INTEGER,
    "cols" INTEGER
, "threshold1" INTEGER, "threshold2" INTEGER);
CREATE UNIQUE INDEX "frames_video_number" on frames (video_id ASC, number ASC);
CREATE INDEX "objects_frame_id" ON "objects" ("frame_id" ASC);
