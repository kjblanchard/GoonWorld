terraform {
  backend "s3" {
    bucket = "supergoon-terraform-plans"
    key    = "supergoonWorld"
    region = "us-east-2"
  }
}